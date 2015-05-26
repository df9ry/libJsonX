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

#ifndef JSONXNUMBER_H_
#define JSONXNUMBER_H_

#include "JsonXValue.h"

namespace JsonX {

class JsonXNumber: public JsonXValue {
public:
	/**
	 * Destructor
	 */
	virtual ~JsonXNumber();

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const = 0;

	/**
	 * Read a number object from input stream. It must be known that
	 * there really is a number, so the first character have to be
	 * a '-' or '0'..'9'.
	 * @param iss The input stream to read from
	 * @return New JsonXNumber allocated on the heap.
	 */
	static JsonXNumber* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const = 0;

	/**
	 * Get value.
	 * @return value
	 */
	int16_t int16Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint16_t uint16Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	int32_t int32Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint32_t uint32Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	int64_t int64Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint64_t uint64Value() const;

	/**
	 * Get value.
	 * @return value
	 */
	double doubleValue() const;

private:
	uint64_t posIntValue(uint64_t max) const;
	int64_t sigIntValue(int64_t min, int64_t max) const;
};

} /* namespace JsonX */

#endif /* JSONXNUMBER_H_ */
