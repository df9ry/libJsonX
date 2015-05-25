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

#include "JsonXValue.h"

namespace JsonX {

/**
 * A JsonXArray holds a list of JsonXValue items.
 */
class JsonXArray: public JsonXValue {
public:

	/**
	 * Construct new empty array.
	 */
	JsonXArray(): m_value{} {}

	/**
	 * Construct new array with an initializer list.
	 * @param il Initializer list
	 */
	JsonXArray(std::initializer_list<JsonXValue*> il): m_value{} {
		for (auto p: il)
			m_value.push_back(move(std::unique_ptr<JsonXValue>(p)));
	}

	/**
	 * Add value to the list with move
	 * @param v Value to add
	 * @return Reference to itself to allow chaining.
	 */
	JsonXArray& add(std::unique_ptr<JsonXValue>&& v) {
		m_value.push_back(move(v));
		return *this;
	}

	/**
	 * Add new value to the list. Takes ownership
	 * @param v Value to add
	 * @return Reference to itself to allow chaining.
	 */
	JsonXArray& add(JsonXValue* v) {
		m_value.push_back(move(std::unique_ptr<JsonXValue>{v}));
		return *this;
	}

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
	 * Destructor
	 */
	virtual ~JsonXArray();

	/**
	 * Read a list from input stream. It must be known that
	 * there really is a list, so the first character have to be
	 * a '['.
	 * @param iss The input stream to read from
	 * @return unique_ptr to JsonXBool
	 */
	static std::unique_ptr<JsonXArray>&& read(std::istream& iss);

private:
	std::vector<std::unique_ptr<JsonXValue>> m_value;
};

} /* namespace JsonX */

#endif /* JSONXARRAY_H_ */
