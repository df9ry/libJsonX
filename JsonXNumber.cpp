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

#include "JsonXNumber.h"
#include "JsonXException.h"

#include <sstream>
#include "JsonXScalar"

using namespace std;

namespace JsonX {

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
            throw JsonXException(
            		"Invalid decimal digit '" + to_string(ch) + "'");
    } // end switch(ch) //
}

JsonXNumber::~JsonXNumber() {}

JsonXNumber* JsonXNumber::read(istream& iss) {
	ostringstream oss{};
    NumberState state = NumberState::START;
    skipWhitespace(iss);
    int next_ch;
    bool isPositive = true;
    bool isDecimal = false;
    while (state != NumberState::DONE)
    {
        next_ch = iss.peek();
        if (next_ch == -1)
            break;
        switch (state)
        {
            case NumberState::START:
                switch (next_ch)
                {
                    case '-' :
                        oss << '-';
                        state = NumberState::AFTER_SIGN;
                        readChar(iss);
                        isPositive = false;
                        break;
                    case '0' :
                        oss << '0';
                        state = NumberState::AFTER_INITIAL_ZERO;
                        readChar(iss);
                        break;
                    default :
                        digit(static_cast<char>(next_ch));
                        oss << static_cast<char>(next_ch);
                        state = NumberState::AFTER_FIRST_DIGIT;
                        readChar(iss);
                        break;
                } // end switch(ch) //
                break;

            case NumberState::AFTER_SIGN:
                switch (next_ch)
                {
                    case '0' :
                        oss << '0';
                        state = NumberState::AFTER_INITIAL_ZERO;
                        readChar(iss);
                        break;
                    default :
                        digit(static_cast<char>(next_ch));
                        oss << static_cast<char>(next_ch);
                        state = NumberState::AFTER_FIRST_DIGIT;
                        readChar(iss);
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
                        oss << '.';
                        state = NumberState::AFTER_PERIOD;
                        readChar(iss);
                        isDecimal = true;
                        break;
                    case 'e':
                    case 'E':
                        oss << 'e';
                        state = NumberState::AFTER_EXPONENT;
                        readChar(iss);
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
                        oss << '.';
                        state = NumberState::AFTER_PERIOD;
                        readChar(iss);
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
                        oss << static_cast<char>(next_ch);
                        readChar(iss);
                        break;
                    case 'e':
                    case 'E':
                        oss << 'e';
                        state = NumberState::AFTER_EXPONENT;
                        readChar(iss);
                        isDecimal = true;
                        break;
                    default :
                        state = NumberState::DONE;
                        break;
                } // end switch(ch) //
                break;

            case NumberState::AFTER_PERIOD:
                digit(static_cast<char>(next_ch));
                oss << static_cast<char>(next_ch);
                state = NumberState::AFTER_DEC_DIGIT;
                readChar(iss);
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
                        oss << static_cast<char>(next_ch);
                        readChar(iss);
                        break;
                    case 'e':
                    case 'E':
                        oss << 'e';
                        state = NumberState::AFTER_EXPONENT;
                        readChar(iss);
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
                        oss << static_cast<char>(next_ch);
                        state = NumberState::AFTER_EXPONENT_SIGN;
                        readChar(iss);
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
                        oss << static_cast<char>(next_ch);
                        state = NumberState::AFTER_EXPONENT_DIGIT;
                        readChar(iss);
                        break;
                    default:
                        throw JsonXException(
                            "Unexpected character '" +
							to_string(static_cast<char>(next_ch)) +
							"' in state " +
							to_string(static_cast<int>(state)));
                } // end switch(ch) //
                break;

            case NumberState::AFTER_EXPONENT_SIGN:
                digit(static_cast<char>(next_ch));
                oss << static_cast<char>(next_ch);
                state = NumberState::AFTER_EXPONENT_DIGIT;
                readChar(iss);
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
                        oss << static_cast<char>(next_ch);
                        readChar(iss);
                        break;
                    default:
                        state = NumberState::DONE;
                        break;
                } // end switch(ch) //
                break;

            default:
                throw JsonXException("Invalid number state " +
					to_string(static_cast<int>(state)));
        } // end switch (state) //
    } // end for //

    if (state != NumberState::DONE)
        throw JsonXException("Premature end of number in state " +
			to_string(static_cast<int>(state)));
    try {
    	string t = oss.str();
    	if (!isDecimal) {
    		if (isPositive) {
    			unsigned long long _v = stoll(t);
    			if (_v <= UINT16_MAX) {
    		        return new JsonXScalar<uint16_t, ValueType::T_UINT16>
    		        	(static_cast<uint16_t>(_v));
    			}
    			if (_v <= UINT32_MAX) {
    		        return new JsonXScalar<uint32_t, ValueType::T_UINT32>
    		        	(static_cast<uint32_t>(_v));
    			}
    			if (_v <= UINT64_MAX) {
    		        return new JsonXScalar<uint64_t, ValueType::T_UINT64>
    		        	(static_cast<uint64_t>(_v));
    			}
    		} else {
    			// is negative:
    			signed long long _v = strtoll(t.c_str(), 0, 0);
    			if (_v >= INT16_MIN) {
    		        return new JsonXScalar<int16_t, ValueType::T_INT16>
    		        	(static_cast<int16_t>(_v));
    			}
    			if (_v >= INT32_MIN) {
    		        return new JsonXScalar<int32_t, ValueType::T_INT32>
    		        	(static_cast<int32_t>(_v));
    			}
    			if (_v >= INT64_MIN) {
    		        return new JsonXScalar<int64_t, ValueType::T_INT64>
    		        	(static_cast<int64_t>(_v));
    			}
    		}
    	}
    	// Here we must handle with double:
    	double _v = stod(t);
        return new JsonXScalar<double, ValueType::T_DOUBLE>
        	(static_cast<double>(_v));
    }
    catch (const exception& ex) {
        throw JsonXException("Invalid number format: ", ex);
    }
    catch (...) {
        throw JsonXException("Invalid number format");
    }
}

uint64_t JsonXNumber::posIntValue(uint64_t max) const {
	switch(type()) {
	case ValueType::T_DOUBLE : {
		double d =
			(static_cast<const JsonXScalar<double, ValueType::T_DOUBLE>*>(this))
			->value() + 0.5;
        if ((d < 0.0) || (d > max)) throw range_error(toString());
        return static_cast<uint64_t>(d);
	}
	case ValueType::T_INT16 : {
		int16_t v =
			(static_cast<const JsonXScalar<int16_t, ValueType::T_INT16>*>(this))
			->value();
        if ((v < 0) || (static_cast<uint64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT16 : {
		uint16_t v =
			(static_cast<const JsonXScalar<uint16_t, ValueType::T_UINT16>*>(this))
			->value();
        if (v > max) throw range_error(toString());
        return v;
	}
	case ValueType::T_INT32 : {
		int32_t v =
			(static_cast<const JsonXScalar<int32_t, ValueType::T_INT32>*>(this))
			->value();
        if ((v < 0) || (static_cast<uint64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT32 : {
		uint32_t v =
			(static_cast<const JsonXScalar<uint32_t, ValueType::T_UINT32>*>(this))
			->value();
        if (v > max) throw range_error(toString());
        return v;
	}
	case ValueType::T_INT64 : {
		int64_t v =
			(static_cast<const JsonXScalar<int64_t, ValueType::T_INT64>*>(this))
			->value();
        if ((v < 0) || (static_cast<uint64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT64 : {
		uint64_t v =
			(static_cast<const JsonXScalar<uint64_t, ValueType::T_UINT64>*>(this))
			->value();
        if (v > max) throw range_error(toString());
        return v;
	}
	default :
		throw JsonXException("JsonXNumber::posIntValue: Invalid type");
	} // end switch //
}

int64_t JsonXNumber::sigIntValue(int64_t min, int64_t max) const {
	switch(type()) {
	case ValueType::T_DOUBLE : {
		double d =
			(static_cast<const JsonXScalar<double, ValueType::T_DOUBLE>*>(this))
			->value() + 0.5;
        if ((d < min) || (d > max)) throw range_error(toString());
        return static_cast<int64_t>(d);
	}
	case ValueType::T_INT16 : {
		int16_t v =
			(static_cast<const JsonXScalar<int16_t, ValueType::T_INT16>*>(this))
			->value();
        if ((v < min) || (v > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT16 : {
		uint16_t v =
			(static_cast<const JsonXScalar<uint16_t, ValueType::T_UINT16>*>(this))
			->value();
        if ((static_cast<int64_t>(v) < min) ||
        	(static_cast<int64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_INT32 : {
		int32_t v =
			(static_cast<const JsonXScalar<int32_t, ValueType::T_INT32>*>(this))
			->value();
        if ((v < min) || (v > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT32 : {
		uint32_t v =
			(static_cast<const JsonXScalar<uint32_t, ValueType::T_UINT32>*>(this))
			->value();
        if ((static_cast<int64_t>(v) < min) ||
        	(static_cast<int64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_INT64 : {
		int64_t v =
			(static_cast<const JsonXScalar<int64_t, ValueType::T_INT64>*>(this))
			->value();
        if ((v < min) || (v > max))
        	throw range_error(toString());
        return v;
	}
	case ValueType::T_UINT64 : {
		uint64_t v =
			(static_cast<const JsonXScalar<uint64_t, ValueType::T_UINT64>*>(this))
			->value();
        if ((v > UINT64_MAX) ||
        	(static_cast<int64_t>(v) < min) ||
        	(static_cast<int64_t>(v) > max))
        	throw range_error(toString());
        return v;
	}
	default :
		throw JsonXException("JsonXNumber::sigIntValue: Invalid type");
	} // end switch //
}

int16_t JsonXNumber::int16Value() const {
	return static_cast<int16_t>(sigIntValue(INT16_MIN, INT16_MAX));
}

uint16_t JsonXNumber::uint16Value() const {
	return static_cast<uint16_t>(posIntValue(UINT16_MAX));
}

int32_t JsonXNumber::int32Value() const {
	return static_cast<int32_t>(sigIntValue(INT32_MIN, INT32_MAX));
}

uint32_t JsonXNumber::uint32Value() const {
	return static_cast<uint32_t>(posIntValue(UINT32_MAX));
}

int64_t JsonXNumber::int64Value() const {
	return sigIntValue(INT64_MIN, INT64_MAX);
}

uint64_t JsonXNumber::uint64Value() const {
	return posIntValue(UINT64_MAX);
}

double JsonXNumber::doubleValue() const {
	if (type() == ValueType::T_DOUBLE) {
		return (static_cast<const JsonXScalar<double, ValueType::T_DOUBLE>*>(this))
				->value();
	}
	return static_cast<double>(sigIntValue(INT64_MIN, INT64_MAX));
}

} /* namespace JsonX */
