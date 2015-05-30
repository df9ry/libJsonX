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

#include "JsonXBool.h"
#include "JsonXException.h"

using namespace std;

namespace JsonX {

JsonXBool::JsonXBool(bool value): m_value{value} {}

JsonXBool::~JsonXBool() {}

string JsonXBool::toString() const {
	string&& s{m_value ? "true" : "false"};
	return move(s);
}

JsonXBool* JsonXBool::read(istream& iss) {
    skipWhitespace(iss);
    bool value;
    if (iss.peek() == 't') {
		if (!(
			(readChar(iss) == 't') &&
			(readChar(iss) == 'r') &&
			(readChar(iss) == 'u') &&
			(readChar(iss) == 'e')
		)) throw JsonXException("Invalid true symbol");
		value = true;
    } else {
		if (!(
			(readChar(iss) == 'f') &&
			(readChar(iss) == 'a') &&
			(readChar(iss) == 'l') &&
			(readChar(iss) == 's') &&
			(readChar(iss) == 'e')
		)) throw JsonXException("Invalid false symbol");
		value = false;
    }
    return new JsonXBool(value);
}

} /* namespace JsonX */
