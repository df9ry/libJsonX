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

#ifndef JSONXBASE_H_
#define JSONXBASE_H_

#include "ValueType.h"

#include <string>
#include <iostream>

namespace JsonX {

/**
 * Base class of all JsonX values.
 */
class JsonXBase {
public:
	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string toString() const = 0;

	/**
	 * Read a value from input stream.
	 * @param iss The input stream to read from
	 * @return Poniter to JsonXValue allocated on heap
	 */
	static JsonXBase* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const = 0;

	/**
	 * Destructor
	 */
	virtual ~JsonXBase() {};

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

#endif /* JSONXBASE_H_ */
