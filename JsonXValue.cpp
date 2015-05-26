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

#include "JsonXValue.h"
#include "JsonXArray.h"
#include "JsonXBlob.h"
#include "JsonXBool.h"
#include "JsonXNull.h"
#include "JsonXNumber.h"
#include "JsonXObject.h"
#include "JsonXString.h"
#include "JsonXException.h"

#include <ctype.h>

using namespace std;

namespace JsonX {

JsonXValue* JsonXValue::read(istream& iss) {
    int next_ch = skipWhitespace(iss);
    switch (next_ch) {
        case -1 :
            return new JsonXNull();
        case '{' :
            return JsonXObject::read(iss);
        case '[' :
            return JsonXArray::read(iss);
        case '-' :
        case '0' :
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
        case '8' :
        case '9' :
            return JsonXNumber::read(iss);
        case '"' :
            return JsonXString::read(iss);
        case '=' :
        	return JsonXBlob::read(iss);
        case 't' :
        case 'f' :
            return JsonXBool::read(iss);
        case 'n' :
            return JsonXNull::read(iss);
        default :
            throw JsonXException(
                "JsonX::read(): Unexpected input: '" +
				to_string(static_cast<char>(next_ch)) + "'");
    } // end switch //
}

int JsonXValue::skipWhitespace(istream& iss) {
	int _ch;
	while (true) {
		_ch = iss.peek();
		if ((_ch != -1) && isspace(_ch))
			readChar(iss);
		else
			return _ch;
	} // end while //
}

} // end namespace JsonX //
