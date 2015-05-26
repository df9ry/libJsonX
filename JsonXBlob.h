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

#include "JsonXValue.h"

#include <vector>
#include <memory>

namespace JsonX {

using JsonXBlobData = typename std::vector<uint8_t>;

class JsonXBlob: public JsonXValue {
public:

	/**
	 * Default constructor.
	 */
	JsonXBlob();

	/**
	 * Constructor.
	 * @param value The blob value
	 */
	JsonXBlob(const JsonXBlobData& value);

	/**
	 * Constructor with move.
	 * @param value The blob value
	 */
	JsonXBlob(JsonXBlobData&& value);

	/**
	 * Support for initializer list
	 * @param il byte values
	 */
	JsonXBlob(std::initializer_list<uint8_t> il);

	/**
	 * Make an new blob on the heap.
	 * @param value The blob value
	 * @return Pointer to object on the heap
	 */
	static inline JsonXBlob* make(const JsonXBlobData& value)
	{ return new JsonXBlob(value); }

	 /**
	 * @param il Byte list
	 * @return New blob on the heap
	 */
	static inline JsonXBlob* make(std::initializer_list<uint8_t> il)
	{ return new JsonXBlob(il); }

	/**
	 * Add byte to the blob
	 * @param x Byte to add
	 * @return Pointer to itself to allow chaining
	 */
	inline JsonXBlob* add(uint8_t x) {
		m_value.get()->push_back(x);
		return this;
	}

	/**
	 * Destructor
	 */
	virtual ~JsonXBlob();

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	virtual std::string&& toString() const;

	/**
	 * Read a blob object from input stream. It must be known that
	 * there really is a blob, so the first character have to be
	 * a '='.
	 * @param iss The input stream to read from
	 * @return New JsonXBlob on the heap
	 */
	static JsonXBlob* read(std::istream& iss);

	/**
	 * Get type of this value, so RTTI is not required.
	 * @return Type of this value
	 */
	virtual ValueType type() const { return ValueType::T_BLOB; }

	/**
	 * Get value.
	 * @return value
	 */
	const JsonXBlobData& value() const { return *m_value.get(); }

	/**
	 * Get value.
	 * @return value
	 */
	JsonXBlobData&& value() { return move(*m_value.get()); }

private:
	std::unique_ptr<JsonXBlobData>&& m_value;
};

} /* namespace JsonX */

#endif /* JSONXBLOB_H_ */
