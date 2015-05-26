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

#include "JsonXNull.h"
#include "JsonXException.h"

using namespace std;

namespace JsonX {

JsonXNull::JsonXNull() {}

JsonXNull::~JsonXNull() {}

string&& JsonXNull::toString() const {
	string&& s{"null"};
	return move(s);
}

JsonXNull* JsonXNull::read(istream& iss) {
    skipWhitespace(iss);
    if (!(
        (readChar(iss) == 'n') &&
        (readChar(iss) == 'u') &&
        (readChar(iss) == 'l') &&
        (readChar(iss) == 'l')
        ))
        throw JsonXException("Invalid null symbol");
    return new JsonXNull();
}

} /* namespace JsonX */
