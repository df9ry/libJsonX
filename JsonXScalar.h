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

#ifndef JSONXSCALAR_H_
#define JSONXSCALAR_H_

#include "JsonXAtom.h"

namespace JsonX {

/**
 * Template for all scalr atoms, like integers and reals
 */
template <typename T, ValueType V>
class JsonXScalar: public JsonXNumber {
public:
	/**
	 * Constructor.
	 * @param value The value
	 */
	JsonXScalar(T value): m_value{value} {};

	/**
	 * Destructor
	 */
	virtual ~JsonXScalar() {};

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const {
		std::string&& s{std::to_string(m_value)};
		return move(s);
	}

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return V; }

	/**
	 * Get value.
	 * @return value
	 */
	T value() const { return m_value; }

private:
	T m_value;
};

} /* namespace JsonX */

#endif /* JSONXSCALAR_H_ */
