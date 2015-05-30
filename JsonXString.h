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

#ifndef JSONXSTRING_H_
#define JSONXSTRING_H_

#include "JsonXBase.h"

namespace JsonX {

class JsonXString: public JsonXBase {
public:
	/**
	 * Constructor.
	 * @param value The string value
	 */
	JsonXString(const std::string& value);

	/**
	 * Helper function to make inline coding easier.
	 * @param value The string value
	 * @return New JsonXString on the heap
	 */
	JsonXString* make(const std::string& value) {
		return new JsonXString(value);
	}

	/**
	 * Destructor
	 */
	virtual ~JsonXString() {};

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string toString() const;

	/**
	 * Read a string object from input stream. It must be known that
	 * there really is a string, so the first character have to be
	 * a '"'.
	 * @param iss The input stream to read from
	 * @return New JsonXString on the heap
	 */
	static JsonXString* read(std::istream& iss);

	/**
	 * Read a json coded string from input stream. It must be known
	 * that there really is a string, so the first character have to
	 * be a '"'.
	 * @param iss The input stream to read from
	 * @return string
	 */
	static std::string readRaw(std::istream& iss);

	/**
	 * Convert string to its json form.
	 * @param s String to convert
	 * @return Converted string
	 */
	static std::string toJson(const std::string& s);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_STRING; }

	/**
	 * Get value.
	 * @return value
	 */
	const std::string& value() const { return m_value; }

private:
	const std::string m_value;
};

} /* namespace JsonX */

#endif /* JSONXSTRING_H_ */
