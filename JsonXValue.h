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

#ifndef JSONXVALUE_H_
#define JSONXVALUE_H_

#include "ValueType.h"
#include "JsonXArray.h"
#include "JsonXBlob.h"
#include "JsonXBool.h"
#include "JsonXNull.h"
#include "JsonXNumber.h"
#include "JsonXObject.h"
#include "JsonXString.h"
#include "JsonXScalar"

#include <string>
#include <iostream>
#include <memory>
#include <vector>

namespace JsonX {

class JsonXBase;

/**
 * Json Value with move semantics.
 */
class JsonXValue {
public:
	/**
	 * Default constructor.
	 */
	JsonXValue();

	/**
	 * Create JsonXValue from a native JsonX data tree. The
	 * ownership will be transfered to this object.
	 * @param v Native JsonX data tree.
	 */
	JsonXValue(JsonXBase* v);

	/**
	 * Copy constructor is deleted.
	 * @param other Not used
	 */
	JsonXValue(const JsonXValue& other) = delete;

	/**
	 * Move constructor.
	 * @param other Move source.
	 */
	JsonXValue(JsonXValue&& other);

	/**
	 * Destructor
	 */
	~JsonXValue();

	/**
	 * Test if the value is set.
	 */
	explicit operator bool() const noexcept { return m_value.get(); }

	/**
	 * Copy assignment is deleted.
	 * @param other Not used.
	 * @return Not used.
	 */
	JsonXValue& operator=(const JsonXValue& other) = delete;

	/**
	 * Move assignment.
	 * @param other Source object.
	 * @return Reference to target object.
	 */
	JsonXValue& operator=(JsonXValue&& other);

	/**
	 * Get string form of Json object.
	 * @return string form of Json object
	 */
	std::string toString() const;

	/**
	 * Get value type.
	 * @return Value type.
	 */
	ValueType type() const;

	/// Support for atomic types:

	/**
	 * Get string value.
	 * @return value.
	 */
	std::string asString() const;

	/**
	 * Get value.
	 * @return value
	 */
	int16_t asInt16() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint16_t asUInt16() const;

	/**
	 * Get value.
	 * @return value
	 */
	int32_t asInt32() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint32_t asUInt32() const;

	/**
	 * Get value.
	 * @return value
	 */
	int64_t asInt64() const;

	/**
	 * Get value.
	 * @return value
	 */
	uint64_t asUInt64() const;

	/**
	 * Get value.
	 * @return value
	 */
	double asDouble() const;

	/// Support for blobs:

	/**
	 * Set blob from initializer list.
	 * @param bytes Literal list of bytes.
	 */
	void setBlob(std::initializer_list<uint8_t> bytes);

	/**
	 * Set blob from byte buffer.
	 * @param pb Pointer to buffer.
	 * @param cb Size of buffer.
	 */
	void setBlob(const uint8_t* pb, size_t cb);

	/**
	 * Set blob from vector.
	 * @param data Blob vector.
	 */
	void setBlob(const JsonXBlobData& data);

	/**
	 * Append blob from initializer list.
	 * @param bytes Literal list of bytes.
	 */
	void appendBlob(std::initializer_list<uint8_t> bytes);

	/**
	 * Append blob from byte buffer.
	 * @param pb Pointer to buffer.
	 * @param cb Size of buffer.
	 */
	void appendBlob(const uint8_t* pb, size_t cb);

	/**
	 * Append blob from vector.
	 * @param data Blob vector.
	 */
	void appendBlob(const JsonXBlobData& data);

	/**
	 * Set blob from vector with move.
	 * @param data Data to move into the blob.
	 */
	void insertBlob(std::unique_ptr<JsonXBlobData>&& data);

	/**
	 * Get blob data.
	 * @return Constant blob data.
	 */
	const JsonXBlobData& getBlob() const;

	/**
	 * Move blob out.
	 * @return Moved blob data.
	 */
	std::unique_ptr<JsonXBlobData> extractBlob();

	/// Support for arrays:

	/**
	 * Set array from vector with move.
	 * @param data Data to move into the array.
	 */
	void insertArray(std::unique_ptr<JsonXArrayData>&& data);

	/**
	 * Get array data.
	 * @return Constant array data.
	 */
	const JsonXArrayData& getArray() const;

	/**
	 * Move array out.
	 * @return Moved array data.
	 */
	std::unique_ptr<JsonXArrayData> extractArray();

	// Support for objects:

	/**
	 * Set object from vector with move.
	 * @param data Data to move into the object.
	 */
	void insertObject(std::unique_ptr<JsonXObjectValue>&& data);

	/**
	 * Get object data.
	 * @return Constant object data.
	 */
	const JsonXObjectValue& getObject() const;

	/**
	 * Move object out.
	 * @return Moved object data.
	 */
	std::unique_ptr<JsonXObjectValue> extractObject();

	/// IO:

	/**
	 * Write a value to input stream.
	 * @param oss The output stream to read from.
	 */
	void write(std::ostream& oss) const;

	/**
	 * Read a value from input stream.
	 * @param iss The input stream to read from.
	 * @return Unique pointer to JsonXValue.
	 */
	static std::unique_ptr<JsonXValue> read(std::istream& iss);

private:
	std::unique_ptr<JsonXBase> m_value;
};

} /* namespace JsonX */

#endif /* JSONXVALUE_H_ */
