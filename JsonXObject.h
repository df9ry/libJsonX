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

#include <string>
#include <vector>
#include <memory>
#include "JsonXBase.h"

namespace JsonX {

class JsonXObject;

/**
 * This is an entry in a json object.
 */
struct JsonXObjectEntry {
	const std::string           key;
	std::unique_ptr<JsonXBase> val;
};

/**
 * This is an entry in an initializer list.
 */
struct JsonXObjectInitEntry {
	const std::string           key;
	JsonXObject*                val;
};

/**
 * Shortcut for value type;
 */
using JsonXObjectValue = std::vector<std::unique_ptr<JsonXObjectEntry>>;

/**
 * This is a json object.
 */
class JsonXObject: public JsonXBase {
public:
	/**
	 * Construct new empty object.
	 */
	JsonXObject();

	/**
	 * Helper function to make inline values more easy.
	 * @return New JsonXObject on the heap
	 */
	JsonXObject* make() { return new JsonXObject(); }

	/**
	 * Initializer list constructor.
	 * @param il The initializer list
	 */
	JsonXObject(std::initializer_list<JsonXObjectInitEntry> il);

	/**
	 * Helper function to make inline values more easy.
	 * @param il The initializer list
	 * @return New JsonXObject on the heap
	 */
	JsonXObject* make(std::initializer_list<JsonXObjectInitEntry> il) {
		return new JsonXObject(il);
	}

	/**
	 * Move another data vector into this object.
	 * @param value Data vector to move in.
	 */
	inline void set(std::unique_ptr<JsonXObjectValue>&& value) {
		std::swap(m_value, value);
	}

	/**
	 * Destructor
	 */
	virtual ~JsonXObject();

	/**
	 * Test if the value is set.
	 */
	explicit operator bool() const noexcept { return (m_value.get() != nullptr); }

	/**
	 * Add value to the object with taking ownership
	 * @param v Value to add
	 * @return Pointer to itself to allow chaining.
	 */
	JsonXObject* add(const std::string& key, JsonXBase* o);

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string toString() const;

	/**
	 * Read an object from input stream. It must be known that
	 * there really is an object, so the first character have to be
	 * a '{'.
	 * @param iss The input stream to read from
	 * @return pointer to new JsonXObject allocated on
	 *         heap
	 */
	static JsonXObject* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_OBJECT; }

	/**
	 * Get value.
	 * @return value
	 */
	const JsonXObjectValue& value() const { return *m_value.get(); }

	/**
	 * Get value.
	 * @return value
	 */
	JsonXObjectValue&& value() { return move(*m_value.get()); }

	/**
	 * Test for a key
	 * @param key The key
	 * @return True, if object contains key
	 */
	bool contains(const std::string& key);

	/**
	 * Find a value by key.
	 * @param key The key
	 * @return Const value or const JsonXNull, if not found
	 */
	const JsonXBase& find(const std::string& key) const;

	JsonXBase&& find(const std::string& key);
	/**
	 * Find a value by key and move out. Throws exception if
	 * not found.
	 * @param key The key
	 * @return Moved value
	 */

	/**
	 * Extract value.
	 * @return value
	 */
	std::unique_ptr<JsonXObjectValue> extract();

private:
	std::unique_ptr<JsonXObjectValue> m_value;
};

} /* namespace JsonX */

#endif /* JSONXOBJECT_H_ */
