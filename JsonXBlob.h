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

#ifndef JSONXBLOB_H_
#define JSONXBLOB_H_

#include "JsonXAtom.h"

#include <vector>

namespace JsonX {

class JsonXBlob: public JsonXAtom {
public:
	/**
	 * Constructor.
	 * @param value The blob value
	 */
	JsonXBlob(const std::vector<uint8_t>& value);

	/**
	 * Constructor with move semantic.
	 * @param value The blob value
	 */
	JsonXBlob(std::unique_ptr<std::vector<uint8_t>>&& value);

	/**
	 * Destructor
	 */
	virtual ~JsonXBlob() {};

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_BLOB; }

	/**
	 * Get value.
	 * @return value
	 */
	const std::vector<uint8_t>& value() const { return *m_value.get(); }

	/**
	 * Get value with move semantic.
	 * @return value
	 */
	std::unique_ptr<std::vector<uint8_t>>&& value() { return std::move(m_value); }

	/**
	 * Read a blob object from input stream. It must be known that
	 * there really is a blob, so the first character have to be
	 * a '='.
	 * @param iss The input stream to read from
	 * @return unique_ptr to JsonXBlob
	 */
	static std::unique_ptr<JsonXBlob>&& read(std::istream& iss);

private:
	std::unique_ptr<std::vector<uint8_t>> m_value;
};

} /* namespace JsonX */

#endif /* JSONXBLOB_H_ */
