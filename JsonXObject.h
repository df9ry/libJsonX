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

#ifndef JSONXOBJECT_H_
#define JSONXOBJECT_H_

#include "JsonXString.h"

#include <string>
#include <vector>

namespace JsonX {

/**
 * This is an entry in a json object.
 */

using JsonXEntry = typename
	std::pair<const std::string, std::unique_ptr<JsonXValue>>;

/**
 * This is a json object.
 */
class JsonXObject: public JsonXValue {
public:
	/**
	 * Construct new empty object.
	 */
	JsonXObject(): m_value{} {}

	/**
	 * Add value to the object with move
	 * @param v Value to add
	 * @return Reference to itself to allow chaining.
	 */
	JsonXObject& add(JsonXEntry&& v) {
		m_value.push_back(move(v));
		return *this;
	}

	/**
	 * Add new value to the list. Takes ownership
	 * @param k Key to add
	 * @param v Value to add
	 * @return Reference to itself to allow chaining.
	 */
	JsonXObject& add(
			const std::string& k,
			std::unique_ptr<JsonXValue>&& v)
	{
		auto pair = make_pair(k, move(v));
		m_value.push_back(move(pair));
		return *this;
	}

	/**
	 * Add new value to the list. Takes ownership
	 * @param k Key to add
	 * @param v Value to add
	 * @return Reference to itself to allow chaining.
	 */
	JsonXObject& add(const std::string& k, JsonXValue* v) {
		JsonXEntry p{JsonXEntry(move(std::string{k}),
				std::unique_ptr<JsonXValue>(v))};
		m_value.push_back(move(p));
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
	virtual ValueType type() const { return ValueType::T_OBJECT; }

	/**
	 * Destructor
	 */
	~JsonXObject();

	/**
	 * Read an object from input stream. It must be known that
	 * there really is an object, so the first character have to be
	 * a '{'.
	 * @param iss The input stream to read from
	 * @return unique_ptr to JsonXBool
	 */
	static std::unique_ptr<JsonXObject>&& read(std::istream& iss);

private:
	std::vector<JsonXEntry> m_value;
};

} /* namespace JsonX */

#endif /* JSONXOBJECT_H_ */
