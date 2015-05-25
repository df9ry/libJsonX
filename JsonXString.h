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

#include "JsonXAtom.h"

namespace JsonX {

class JsonXString: public JsonXAtom {
public:
	/**
	 * Constructor.
	 * @param value The string value
	 */
	JsonXString(const std::string& value);

	/**
	 * Constructor with move semantic.
	 * @param value The string value
	 */
	JsonXString(std::unique_ptr<std::string>&& value);

	/**
	 * Destructor
	 */
	virtual ~JsonXString() {};

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_STRING; }

	/**
	 * Get value.
	 * @return value
	 */
	const std::string& value() const { return *m_value.get(); }

	/**
	 * Get value with move semantic.
	 * @return value
	 */
	std::unique_ptr<std::string>&& value() { return std::move(m_value); }

	/**
	 * Read a string object from input stream. It must be known that
	 * there really is a string, so the first character have to be
	 * a '"'.
	 * @param iss The input stream to read from
	 * @return unique_ptr to JsonXString
	 */
	static std::unique_ptr<JsonXString>&& read(std::istream& iss) {
	    return move(std::unique_ptr<JsonXString>(
	    		new JsonXString(move(readRaw(iss)))));
	}

	/**
	 * Read a json coded string from input stream. It must be known
	 * that there really is a string, so the first character have to
	 * be a '"'.
	 * @param iss The input stream to read from
	 * @return string
	 */
	static std::string&& readRaw(std::istream& iss);

	/**
	 * Convert string to its json form.
	 * @param s String to convert
	 * @return Converted string
	 */
	static std::string&& toJson(const std::string& s);

private:
	std::unique_ptr<std::string> m_value;
};

} /* namespace JsonX */

#endif /* JSONXSTRING_H_ */
