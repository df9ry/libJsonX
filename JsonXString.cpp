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

#include "JsonXString.h"
#include "JsonXException.h"

#include <ctype.h>
#include <sstream>

using namespace std;

namespace JsonX {

enum class StringState {
    NORMAL,
    ESCAPE,
    U0, U1, U2, U3,
    STRING_CLOSED
};

static uint8_t hex_digit(char ch) {
    switch (ch) {
        case '0' :
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
        default:
            throw JsonXException("Invalid hex digit '" +
            		to_string(ch) + "'");
    } // end switch(ch) //
}

JsonXString::JsonXString(const string& value):
	m_value{unique_ptr<string>{new string(value)}} {}

JsonXString::JsonXString(unique_ptr<string>&& value):
	m_value{unique_ptr<string>{move(value)}} {}

string&& JsonXString::toString() const {
	return move(toJson(value()));
}

string&& JsonXString::toJson(const string& s) {
	ostringstream oss{};
    oss << '"';
    for (string::const_iterator i = s.begin();
    		i != s.end(); ++i)
    {
    	char ch = *i;
        switch (ch)
        {
            case '"' :
                oss << "\\\"";
                break;
            case '\\':
                oss << "\\\\";
                break;
            case '/':
                oss << "\\/";
                break;
            case '\b':
                oss << "\\b";
                break;
            case '\f':
                oss << "\\f";
                break;
            case '\n':
                oss << "\\n";
                break;
            case '\r':
                oss << "\\r";
                break;
            case '\t':
                oss << "\\t";
                break;
            default:
                if (iscntrl(ch))
                {
                	char b[7];
                	snprintf(b, sizeof(b), "\\u%04x", static_cast<int>(ch));
                    oss << b;
                }
                else
                {
                    oss << ch;
                }
                break;
        } // end switch //
    } // end for //
    oss << '"';
	return move(oss.str());
}

string&& JsonXString::readRaw(istream& iss) {
    ostringstream oss{};
    StringState state = StringState::NORMAL;
    uint u = 0;
    int next_ch = skipWhitespace(iss);
    if (next_ch != '"')
        throw JsonXException(
            "Invalid input: Expected '\"', got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
    readChar(iss);
    while (state != StringState::STRING_CLOSED) {
        next_ch = iss.peek();
        if (next_ch == -1)
            break;
        switch (state) {
            case StringState::NORMAL :
                switch (next_ch) {
                    case '\\' :
                        state = StringState::ESCAPE;
                        readChar(iss);
                        break;
                    case '"' :
                        state = StringState::STRING_CLOSED;
                        readChar(iss);
                        break;
                    default :
                        oss << static_cast<char>(next_ch);
                        readChar(iss);
                        break;
                } // end switch(ch) //
                break;

            case StringState::ESCAPE :
                switch (next_ch) {
                    case '"' :
                        oss << '"';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case '\\' :
                        oss << '\\';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case '/':
                        oss << '/';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 'b':
                        oss << '\b';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 'f':
                        oss << '\f';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 'n':
                        oss << '\n';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 'r':
                        oss << '\r';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 't':
                        oss << '\t';
                        state = StringState::NORMAL;
                        readChar(iss);
                        break;
                    case 'u':
                        oss << '\b';
                        state = StringState::U0;
                        readChar(iss);
                        break;
                    default :
                        throw JsonXException(
                            "Invalid escape sequence \"\\" +
                            to_string(static_cast<char>(next_ch)) + "\"");
                } // end switch(ch)
                break;
            case StringState::U0 :
                u = hex_digit(static_cast<char>(next_ch));
                state = StringState::U1;
                readChar(iss);
                break;
            case StringState::U1:
                u = (16 * u) + hex_digit(static_cast<char>(next_ch));
                state = StringState::U2;
                readChar(iss);
                break;
            case StringState::U2:
                u = (16 * u) + hex_digit(static_cast<char>(next_ch));
                state = StringState::U3;
                readChar(iss);
                break;
            case StringState::U3:
                u = (16 * u) + hex_digit(static_cast<char>(next_ch));
                oss << static_cast<char>(u);
                state = StringState::NORMAL;
                readChar(iss);
                break;
            default:
                throw JsonXException("ReadString: Invalid state " +
					to_string(static_cast<int>(state)));
        } // end switch(state) //
    } // end for //
    if (state != StringState::STRING_CLOSED)
        throw JsonXException("String not closed in state " +
				to_string(static_cast<int>(state)));
    return move(oss.str());
}

} /* namespace JsonX */
