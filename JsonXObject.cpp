/*
    Project libJsonX
    Copyright (C) 2015  tania@df9ry.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JsonXObject.h"
#include "JsonXException.h"

#include <sstream>
#include <iostream>

using namespace std;

namespace JsonX {

string&& JsonXObject::toString() const {
	ostringstream oss{};
	oss << '{';
	bool first{true};
	oss << '[';
	for (vector<JsonXEntry>::const_iterator i = m_value.begin();
			i != m_value.end(); ++i) {
		if (first) first = false; else oss << ',';
		oss << JsonXString::toJson(i->first);
		oss << ':';
		oss << i->second.get()->toString();
	} // end for //
	oss << '}';
	return move(oss.str());
}

unique_ptr<JsonXObject>&& JsonXObject::read(istream& iss) {
    int next_ch;
    next_ch = (char)skipWhitespace(iss);
    if (next_ch == -1)
        throw JsonXException(
        		"JsonXObject::read(): Premature end of file");
    if (next_ch != '{')
        throw JsonXException(
            "Character '{' expected. Got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
    auto items = unique_ptr<JsonXObject>(new JsonXObject());
    readChar(iss);;
    next_ch = skipWhitespace(iss);
    if (next_ch != '}') {
        while (true) {
            string&& key = move(JsonXString::readRaw(iss));
            next_ch = skipWhitespace(iss);
            if (next_ch != ':')
                throw JsonXException(
                    "Expected ':'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
            readChar(iss);;
            unique_ptr<JsonXValue>&& val = JsonXValue::read(iss);
            items.get()->add(key, move(val));
            next_ch = skipWhitespace(iss);
            if (next_ch == '}')
                break;
            if (next_ch != ',')
                throw JsonXException(
                    "Expected ',' or '}'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
            readChar(iss);;
        } // end while //
    }
    readChar(iss);
    return move(items);
}

} /* namespace JsonX */
