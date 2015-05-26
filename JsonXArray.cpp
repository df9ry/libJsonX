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

#include "JsonXArray.h"

#include "JsonXException.h"

#include <sstream>
#include <iostream>

using namespace std;

namespace JsonX {

JsonXArray::JsonXArray(): m_value{
	unique_ptr<JsonXArrayData>(new JsonXArrayData())} {}

JsonXArray::JsonXArray(initializer_list<JsonXValue*> il):
	m_value{unique_ptr<JsonXArrayData>(new JsonXArrayData(il.size()))}
{
	uninitialized_copy(il.begin(), il.begin() + il.size(),
			m_value.get()->begin());
}

JsonXArray::~JsonXArray() {}

JsonXArray* JsonXArray::add(JsonXValue* v) {
	value().push_back(unique_ptr<JsonXValue>{move(v)});
	return this;
}

string&& JsonXArray::toString() const {
	ostringstream oss{};
	bool first{true};
	oss << '[';
	for (JsonXArrayData::const_iterator i = value().begin();
			i != value().end(); ++i)
	{
		if (first) first = false; else oss << ',';
		JsonXValue* v{i->get()};
		oss << v ? v->toString() : "null";
	} // end for //
	oss << ']';
	return move(oss.str());
}

JsonXArray* JsonXArray::read(istream& iss) {
    int next_ch;
    next_ch = skipWhitespace(iss);
    if (next_ch == -1)
        throw JsonXException(
        		"JsonXArray.ReadArray(): Premature end of file");
    if (next_ch != '[')
        throw JsonXException(
            "Character '[' expected. Got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
    auto pArray = new JsonXArray();
    readChar(iss);
    next_ch = skipWhitespace(iss);
    if (next_ch != ']') {
        while (true) {
            pArray->add(JsonXValue::read(iss));
            next_ch = skipWhitespace(iss);
            if (next_ch == ']')
                break;
            if (next_ch != ',')
                throw JsonXException(
                    "Expected ',' or ']'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
            readChar(iss);
        } // end while //
    }
    readChar(iss);
    return pArray;
}

} /* namespace JsonX */
