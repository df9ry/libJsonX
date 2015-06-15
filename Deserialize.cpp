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

#include <B64Coder.h>

#include <sstream>
#include <stdexcept>

using namespace std;

namespace JsonX {

	static inline int readChar(istream& is) {
		char b[1];
		return (is.readsome(b, 1) == 1) ? b[0] : -1;
	}

	static int skipWhitespace(istream& is) {
		int _ch;
		while (true) {
			_ch = is.peek();
			if ((_ch != -1) && isspace(_ch))
				readChar(is);
			else
				return _ch;
		} // end while //
	}

	enum class NumberState {
		START,
		AFTER_SIGN,
		AFTER_FIRST_DIGIT,
		AFTER_INITIAL_ZERO,
		AFTER_PERIOD,
		AFTER_DEC_DIGIT,
		AFTER_EXPONENT,
		AFTER_EXPONENT_SIGN,
		AFTER_EXPONENT_DIGIT,
		DONE
	};

	static uint8_t digit(char ch) {
		switch (ch) {
		case '0':
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
		default:
			throw runtime_error(
				"Invalid decimal digit '" + to_string(ch) + "'");
		} // end switch(ch) //
	}

	enum class StringState {
		NORMAL,
		ESCAPE,
		U0, U1, U2, U3,
		STRING_CLOSED
	};

	static uint8_t hex_digit(char ch) {
		switch (ch) {
		case '0':
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
			throw runtime_error("Invalid hex digit '" + to_string(ch) + "'");
		} // end switch(ch) //
	}

	AtomPtr Atom::fromJson(std::istream& is) {
		int next_ch = skipWhitespace(is);
		switch (next_ch) {
		case 'n': {
			readChar(is);
			if ((readChar(is) == 'u') && (readChar(is) == 'l') && (readChar(is) == 'l'))
				return Atom::make();
			else
				throw runtime_error("Syntax error");
		}
		case 't': {
			readChar(is);
			if ((readChar(is) == 'r') && (readChar(is) == 'u') && (readChar(is) == 'e'))
				return Atom::make(true);
			else
				throw runtime_error("Syntax error");
		}
		case 'f': {
			readChar(is);
			if ((readChar(is) == 'a') && (readChar(is) == 'l') && (readChar(is) == 's') && (readChar(is) == 'e'))
				return Atom::make(false);
			else
				throw runtime_error("Syntax error");
		}
		default:
			break;
		} // end switch //
		ostringstream os{};
		NumberState state = NumberState::START;
		bool isPositive = true;
		bool isDecimal = false;
		while (state != NumberState::DONE)
		{
			next_ch = is.peek();
			if (next_ch == -1)
				break;
			switch (state)
			{
			case NumberState::START:
				switch (next_ch)
				{
				case '-':
					os << '-';
					state = NumberState::AFTER_SIGN;
					readChar(is);
					isPositive = false;
					break;
				case '0':
					os << '0';
					state = NumberState::AFTER_INITIAL_ZERO;
					readChar(is);
					break;
				default:
					digit(static_cast<char>(next_ch));
					os << static_cast<char>(next_ch);
					state = NumberState::AFTER_FIRST_DIGIT;
					readChar(is);
					break;
				} // end switch(ch) //
				break;

			case NumberState::AFTER_SIGN:
				switch (next_ch)
				{
				case '0':
					os << '0';
					state = NumberState::AFTER_INITIAL_ZERO;
					readChar(is);
					break;
				default:
					digit(static_cast<char>(next_ch));
					os << static_cast<char>(next_ch);
					state = NumberState::AFTER_FIRST_DIGIT;
					readChar(is);
					break;
				} // end switch(ch) //
				break;

			case NumberState::AFTER_INITIAL_ZERO:
				switch (next_ch)
				{
				case -1:
					state = NumberState::DONE;
					break;
				case '.':
					os << '.';
					state = NumberState::AFTER_PERIOD;
					readChar(is);
					isDecimal = true;
					break;
				case 'e':
				case 'E':
					os << 'e';
					state = NumberState::AFTER_EXPONENT;
					readChar(is);
					isDecimal = true;
					break;
				default:
					state = NumberState::DONE;
					break;
				} // end switch(ch) //
				break;

			case NumberState::AFTER_FIRST_DIGIT:
				switch (next_ch)
				{
				case -1:
					state = NumberState::DONE;
					break;
				case '.':
					os << '.';
					state = NumberState::AFTER_PERIOD;
					readChar(is);
					isDecimal = true;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					os << static_cast<char>(next_ch);
					readChar(is);
					break;
				case 'e':
				case 'E':
					os << 'e';
					state = NumberState::AFTER_EXPONENT;
					readChar(is);
					isDecimal = true;
					break;
				default:
					state = NumberState::DONE;
					break;
				} // end switch(ch) //
				break;

			case NumberState::AFTER_PERIOD:
				digit(static_cast<char>(next_ch));
				os << static_cast<char>(next_ch);
				state = NumberState::AFTER_DEC_DIGIT;
				readChar(is);
				break;

			case NumberState::AFTER_DEC_DIGIT:
				switch (next_ch)
				{
				case -1:
					state = NumberState::DONE;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					os << static_cast<char>(next_ch);
					readChar(is);
					break;
				case 'e':
				case 'E':
					os << 'e';
					state = NumberState::AFTER_EXPONENT;
					readChar(is);
					isDecimal = true;
					break;
				default:
					state = NumberState::DONE;
					break;
				} // end switch(ch) //
				break;

			case NumberState::AFTER_EXPONENT:
				switch (next_ch)
				{
				case '+':
				case '-':
					os << static_cast<char>(next_ch);
					state = NumberState::AFTER_EXPONENT_SIGN;
					readChar(is);
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					os << static_cast<char>(next_ch);
					state = NumberState::AFTER_EXPONENT_DIGIT;
					readChar(is);
					break;
				default:
					throw runtime_error(
						"Unexpected character '" +
						to_string(static_cast<char>(next_ch)) +
						"' in state " +
						to_string(static_cast<int>(state)));
				} // end switch(ch) //
				break;

			case NumberState::AFTER_EXPONENT_SIGN:
				digit(static_cast<char>(next_ch));
				os << static_cast<char>(next_ch);
				state = NumberState::AFTER_EXPONENT_DIGIT;
				readChar(is);
				break;

			case NumberState::AFTER_EXPONENT_DIGIT:
				switch (next_ch)
				{
				case -1:
					state = NumberState::DONE;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					os << static_cast<char>(next_ch);
					readChar(is);
					break;
				default:
					state = NumberState::DONE;
					break;
				} // end switch(ch) //
				break;

			default:
				throw runtime_error("Invalid number state " +
					to_string(static_cast<int>(state)));
			} // end switch (state) //
		} // end for //

		if (state != NumberState::DONE)
			throw runtime_error("Premature end of number in state " +
			to_string(static_cast<int>(state)));
		try {
			string t = os.str();
			if (!isDecimal) {
				if (isPositive) {
					unsigned long long _v = stoll(t);
					return Atom::make(static_cast<uint64_t>(_v));
				}
				else {
					// is negative:
					signed long long _v = strtoll(t.c_str(), 0, 0);
					return Atom::make(static_cast<int64_t>(_v));
				}
			}
			// Here we must handle with double:
			double _v = stod(t);
			return Atom::make(_v);
		}
		catch (const exception& ex) {
			throw runtime_error(string("Invalid number format: ") + ex.what());
		}
		catch (...) {
			throw runtime_error("Invalid number format");
		}
	}

	BlobPtr Blob::fromJson(std::istream& is) {
		int next_ch = skipWhitespace(is);
		if (next_ch != '=')
			throw runtime_error(
			"Invalid input: Expected '=', got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
		readChar(is);
		unique_ptr<BlobValue> pv{ new BlobValue() };
		B64::decode([&is](){ char ch; return is.readsome(&ch, 1) ? ch : -1; },
			[&pv](uint8_t x){ pv.get()->push_back(x); });
		// Read away trailing '=':
		while (is.peek() == '=')
			readChar(is);
		return Blob::make(pv);
	}

	string String::fromJsonRaw(std::istream& is) {
		ostringstream oss{};
		StringState state = StringState::NORMAL;
		unsigned int u = 0;
		int next_ch = skipWhitespace(is);
		if (next_ch != '"')
			throw runtime_error(
			"Invalid input: Expected '\"', got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
		readChar(is);
		while (state != StringState::STRING_CLOSED) {
			next_ch = is.peek();
			if (next_ch == -1)
				break;
			switch (state) {
			case StringState::NORMAL:
				switch (next_ch) {
				case '\\':
					state = StringState::ESCAPE;
					readChar(is);
					break;
				case '"':
					state = StringState::STRING_CLOSED;
					readChar(is);
					break;
				default:
					oss << static_cast<char>(next_ch);
					readChar(is);
					break;
				} // end switch(ch) //
				break;

			case StringState::ESCAPE:
				switch (next_ch) {
				case '"':
					oss << '"';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case '\\':
					oss << '\\';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case '/':
					oss << '/';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 'b':
					oss << '\b';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 'f':
					oss << '\f';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 'n':
					oss << '\n';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 'r':
					oss << '\r';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 't':
					oss << '\t';
					state = StringState::NORMAL;
					readChar(is);
					break;
				case 'u':
					oss << '\b';
					state = StringState::U0;
					readChar(is);
					break;
				default:
					throw runtime_error(
						"Invalid escape sequence \"\\" +
						to_string(static_cast<char>(next_ch)) + "\"");
				} // end switch(ch)
				break;
			case StringState::U0:
				u = hex_digit(static_cast<char>(next_ch));
				state = StringState::U1;
				readChar(is);
				break;
			case StringState::U1:
				u = (16 * u) + hex_digit(static_cast<char>(next_ch));
				state = StringState::U2;
				readChar(is);
				break;
			case StringState::U2:
				u = (16 * u) + hex_digit(static_cast<char>(next_ch));
				state = StringState::U3;
				readChar(is);
				break;
			case StringState::U3:
				u = (16 * u) + hex_digit(static_cast<char>(next_ch));
				oss << static_cast<char>(u);
				state = StringState::NORMAL;
				readChar(is);
				break;
			default:
				throw runtime_error("ReadString: Invalid state " +
					to_string(static_cast<int>(state)));
			} // end switch(state) //
		} // end for //
		if (state != StringState::STRING_CLOSED)
			throw runtime_error("String not closed in state " +
			to_string(static_cast<int>(state)));
		return oss.str();
	}

	StringPtr String::fromJson(std::istream& is) {
		return String::make(String::fromJsonRaw(is));
	}

	ListPtr List::fromJson(std::istream& is) {
		int next_ch = skipWhitespace(is);
		if (next_ch == -1)
			throw runtime_error(
			"JsonXArray.ReadArray(): Premature end of file");
		if (next_ch != '[')
			throw runtime_error(
			"Character '[' expected. Got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
		unique_ptr<ListValue> pv{ new ListValue() };
		readChar(is);
		next_ch = skipWhitespace(is);
		if (next_ch != ']') {
			while (true) {
				pv.get()->push_back(Value::fromJson(is));
				next_ch = skipWhitespace(is);
				if (next_ch == ']')
					break;
				if (next_ch != ',')
					throw runtime_error(
					"Expected ',' or ']'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
				readChar(is);
			} // end while //
		}
		readChar(is);
		return List::make(move(pv));
	}

	ObjectPtr Object::fromJson(std::istream& is) {
		int next_ch = skipWhitespace(is);
		if (next_ch == -1)
			throw runtime_error(
			"JsonXObject::read(): Premature end of file");
		if (next_ch != '{')
			throw runtime_error(
			"Character '{' expected. Got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
		unique_ptr<ObjectValue> pv{ new ObjectValue() };
		readChar(is);;
		next_ch = skipWhitespace(is);
		if (next_ch != '}') {
			while (true) {
				string key{ String::fromJsonRaw(is) };
				next_ch = skipWhitespace(is);
				if (next_ch != ':')
					throw runtime_error(
					"Expected ':'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
				readChar(is);
				pv.get()->push_back(ObjectEntry{ key, Value::fromJson(is) });
				next_ch = skipWhitespace(is);
				if (next_ch == '}')
					break;
				if (next_ch != ',')
					throw runtime_error(
					"Expected ',' or '}'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
				readChar(is);
			} // end while //
		}
		readChar(is);
		return Object::make(move(pv));
	}

	ValuePtr Value::fromJson(const std::string& s) {
		return Value::fromJson(stringstream{ s });
	}

	ValuePtr Value::fromJson(std::istream& is) {
		int next_ch = skipWhitespace(is);
		switch (next_ch) {
		case -1:
			return Value::make();
		case '{':
			return Object::fromJson(is);
		case '[':
			return List::fromJson(is);
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 't':
		case 'f':
		case 'n':
			return Atom::fromJson(is);
		case '"':
			return String::fromJson(is);
		case '=':
			return Blob::fromJson(is);
		default:
			throw runtime_error(
				"JsonX::read(): Unexpected input: '" +
				to_string(static_cast<char>(next_ch)) + "'");
		} // end switch //
	}

} // end namespace JsonX //
