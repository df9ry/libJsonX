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

#ifndef JSONXARRAY_H_
#define JSONXARRAY_H_

#include <string>
#include <vector>
#include <memory>

#include "JsonXBase.h"
#include "JsonXObject.h"

namespace JsonX {

using JsonXArrayData = typename
	std::vector<std::unique_ptr<JsonXBase>>;

/**
 * A JsonXArray holds a list of JsonXValue items.
 */
class JsonXArray: public JsonXBase {
public:

	/**
	 * Construct new empty array.
	 */
	JsonXArray();

	/**
	 * Comfort function to create a JsonXArray.
	 * @return Pointer to new object on the heap
	 */
	static inline JsonXArray* make()
	{ return new JsonXArray(); }

	/**
	 * Construct new array with an initializer list.
	 * @param il Initializer list
	 */
	JsonXArray(std::initializer_list<JsonXBase*> il);

	/**
	 * Destructor
	 */
	virtual ~JsonXArray();

	/**
	 * Add new value to the list. Takes ownership
	 * @param v Value to add
	 * @return Pointer to itself to allow chaining.
	 */
	JsonXArray* add(JsonXBase* v);

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_ARRAY; }

	/**
	 * Read a list from input stream. It must be known that
	 * there really is a list, so the first character have to be
	 * a '['.
	 * @param iss The input stream to read from
	 * @return Pointer to object on the heap
	 */
	static JsonXArray* read(std::istream& iss);

	/**
	 * Get value.
	 * @return value
	 */
	const JsonXArrayData& value() const { return *m_value.get(); }

	/**
	 * Get value.
	 * @return value
	 */
	JsonXArrayData&& value() { return move(*m_value.get()); }

private:
	std::unique_ptr<JsonXArrayData> m_value;
};

} /* namespace JsonX */

#endif /* JSONXARRAY_H_ */
