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

#ifndef JSONXVALUE_H_
#define JSONXVALUE_H_

#include <string>
#include <memory>
#include <iostream>

namespace JsonX {

/**
 * Class marker, to avoid RTTI.
 */
enum class ValueType {
	T_STRING,    //!< T_STRING  This is a json string
	T_BLOB,      //!< T_BLOB    This is a json blob (Extension)
	T_NULL,      //!< T_NULL    This is a null value
	T_BOOL,      //!< T_BOOL    This is a json boolean
	T_INT16,     //!< T_INT16   This is a json 16 bit signed integer
	T_UINT16,    //!< T_UINT16  This is a json 16 bit unsigned integer
	T_INT32,     //!< T_INT32   This is a json 32 bit signed integer
	T_UINT32,    //!< T_UINT32  This is a json 32 bit unsigned integer
	T_INT64,     //!< T_INT64   This is a json 64 bit signed integer
	T_UINT64,    //!< T_UINT64  This is a json 64 bit unsigned integer
	T_DOUBLE,    //!< T_DOUBLE  This is a json double real
	T_OBJECT,    //!< T_OBJECT  This is a json object
	T_ARRAY,     //!< T_ARRAY   This is a json array
};

/**
 * Base class of all JsonX values.
 */
class JsonXValue {
public:
	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const = 0;

	/**
	 * Read a value from input stream.
	 * @param iss The input stream to read from
	 * @return unique_ptr to JsonXBool
	 */
	static std::unique_ptr<JsonXValue>&& read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const = 0;

	/**
	 * Destructor
	 */
	virtual ~JsonXValue();

	/**
	 * Skip whitespace from istream.
	 * @param iss The istringstream to use.
	 * @return First char as int that is not whitespace or -1
	 *         if eos.
	 */
	static int skipWhitespace(std::istream& iss);

	/**
	 * Read one char
	 * @param iss The istream to read from
	 * @return Next char or -1 on eos
	 */
	static inline int readChar(std::istream& iss) {
		char b;
		return iss.readsome(&b, 1) ? static_cast<int>(b) : -1;
	}
};

} /* namespace JsonX */

#endif /* JSONXVALUE_H_ */
