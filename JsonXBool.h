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

#ifndef JSONXBOOL_H_
#define JSONXBOOL_H_

#include "JsonXValue.h"

namespace JsonX {

class JsonXBool: public JsonXValue {
public:
	/**
	 * Constructor.
	 * @param value The bool value
	 */
	JsonXBool(bool value);

	/**
	 * Comfort function for constructing in code
	 * @param value The bool value
	 * @return New JsonXBool allocated on the heap
	 */
	JsonXBool* make(bool value) { return new JsonXBool(value); }

	/**
	 * Destructor
	 */
	virtual ~JsonXBool();

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Read a bool object from input stream. It must be known that
	 * there really is a bool, so the first character have to be
	 * a 't' or a 'f'.
	 * @param iss The input stream to read from
	 * @return Pointer to new JsonXBool allocated on the heap
	 */
	static JsonXBool* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_BOOL; }

	/**
	 * Get value.
	 * @return value
	 */
	bool value() const { return m_value; }

private:
	const bool m_value;
};

} /* namespace JsonX */

#endif /* JSONXBOOL_H_ */
