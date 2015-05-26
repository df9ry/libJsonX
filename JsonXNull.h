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

#ifndef JSONXNULL_H_
#define JSONXNULL_H_

#include "JsonXValue.h"

namespace JsonX {

class JsonXNull: public JsonXValue {
public:
	/**
	 * Constructor.
	 */
	JsonXNull();

	/**
	 * Comfort function to make inline coding easier.
	 * @return New JsonXNull on the heap
	 */
	JsonXNull* make() { return new JsonXNull(); }

	/**
	 * Destructor
	 */
	virtual ~JsonXNull();

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Read a null object from input stream. It must be known that
	 * there really is a null, so the first character have to be
	 * a 'n'.
	 * @param iss The input stream to read from
	 * @return New JsonXNull object on the heap
	 */
	static JsonXNull* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_NULL; }

};

} /* namespace JsonX */

#endif /* JSONXNULL_H_ */
