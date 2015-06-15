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

#include "JsonXValue.h"

using namespace std;

namespace JsonX {

	const std::string TypeString(Type t) {
		switch (t) {
		case Type::NONE: return "NONE";
		case Type::SIGNED: return "SIGNED";
		case Type::UNSIGNED: return "UNSIGNED";
		case Type::BOOL: return "BOOL";
		case Type::FLOAT: return "FLOAT";
		case Type::BLOB: return "BLOB";
		case Type::STRING: return "STRING";
		case Type::LIST: return "LIST";
		case Type::OBJECT: return "OBJECT";
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(t));
		} // end switch //
	}

	static const BlobValue emptyBlobValue{};
	static const string emptyStringValue{};
	static const ListValue emptyListValue{};
	static const ObjectValue emptyObjectValue{};

	bool Value::isEmpty() const {
		switch (m_type) {
		case Type::NONE:
			return true;
		case Type::SIGNED:
		case Type::UNSIGNED:
		case Type::BOOL:
		case Type::FLOAT:
			return false;
		case Type::BLOB:
			return (v_blob == nullptr);
		case Type::STRING:
			return (v_string == nullptr);
		case Type::LIST:
			return (v_list == nullptr);
		case Type::OBJECT:
			return (v_object == nullptr);
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	Atom& Value::asAtom() {
		if (isAtom()) return *static_cast<Atom*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Atom");
	}

	Blob& Value::asBlob() {
		if (isBlob()) return *static_cast<Blob*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Blob");
	}

	String& Value::asString() {
		if (isString()) return *static_cast<String*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to String");
	}

	List& Value::asList() {
		if (isList()) return *static_cast<List*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to List");
	}

	Object& Value::asObject() {
		if (isObject()) return *static_cast<Object*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Object");
	}

	const Atom& Value::asConstAtom() const {
		if (isAtom()) return *static_cast<const Atom*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Atom");
	}

	const Blob& Value::asConstBlob() const {
		if (isBlob()) return *static_cast<const Blob*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Blob");
	}

	const String& Value::asConstString() const {
		if (isString()) return *static_cast<const String*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to String");
	}

	const List& Value::asConstList() const {
		if (isList()) return *static_cast<const List*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to List");
	}

	const Object& Value::asConstObject() const {
		if (isObject()) return *static_cast<const Object*>(this);
		throw std::runtime_error("Can not convert " + TypeString(m_type) + " to Object");
	}

	void Value::reset() {
		switch (m_type) {
		case Type::NONE:
			return;
		case Type::SIGNED:
			v_signed = 0;
			return;
		case Type::UNSIGNED:
			v_unsigned = 0;
			return;
		case Type::BOOL:
			v_bool = false;
			return;
		case Type::FLOAT:
			v_float = 0.0;
			return;
		case Type::BLOB:
			if (v_blob != nullptr) {
				delete v_blob;
				v_blob = nullptr;
			}
			return;
		case Type::STRING:
			if (v_string != nullptr) {
				delete v_string;
				v_string = nullptr;
			}
			return;
		case Type::LIST:
			if (v_list != nullptr) {
				delete v_list;
				v_list = nullptr;
			}
			return;
		case Type::OBJECT:
			if (v_object != nullptr) {
				delete v_object;
				v_object = nullptr;
			}
			return;
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	Value::Value(Value&& other) {
		m_type = other.m_type;
		other.m_type = Type::NONE;
		switch (m_type) {
		case Type::NONE:
			return;
		case Type::SIGNED:
			v_signed = other.v_signed;
			other.v_signed = 0;
			return;
		case Type::UNSIGNED:
			v_unsigned = other.v_unsigned;
			other.v_unsigned = 0;
			return;
		case Type::BOOL:
			v_bool = other.v_bool;
			other.v_bool = false;
			return;
		case Type::FLOAT:
			v_float = other.v_float;
			other.v_float = 0.0;
			return;
		case Type::BLOB:
			v_blob = other.v_blob;
			other.v_blob = nullptr;
			return;
		case Type::STRING:
			v_string = other.v_string;
			other.v_string = nullptr;
			return;
		case Type::LIST:
			v_list = other.v_list;
			other.v_list = nullptr;
			return;
		case Type::OBJECT:
			v_object = other.v_object;
			other.v_object = nullptr;
			return;
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	Value& Value::operator=(Value&& other) {
		reset();
		m_type = other.m_type;
		other.m_type = Type::NONE;
		switch (m_type) {
		case Type::NONE:
			return *this;
		case Type::SIGNED:
			v_signed = other.v_signed;
			other.v_signed = 0;
			return *this;
		case Type::UNSIGNED:
			v_unsigned = other.v_unsigned;
			other.v_unsigned = 0;
			return *this;
		case Type::BOOL:
			v_bool = other.v_bool;
			other.v_bool = false;
			return *this;
		case Type::FLOAT:
			v_float = other.v_float;
			other.v_float = 0.0;
			return *this;
		case Type::BLOB:
			v_blob = other.v_blob;
			other.v_blob = nullptr;
			return *this;
		case Type::STRING:
			v_string = other.v_string;
			other.v_string = nullptr;
			return *this;
		case Type::LIST:
			v_list = other.v_list;
			other.v_list = nullptr;
			return *this;
		case Type::OBJECT:
			v_object = other.v_object;
			other.v_object = nullptr;
			return *this;
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	void* Value::operator new(size_t sz){
		return ::std::malloc(sz);
	}

	void* Value::operator new[](size_t sz) {
		return ::operator new(sz);
	}

	const Value Value::empty;

	std::uint8_t Atom::asUInt8() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned < 0) throw range_error("Value is negative");
			if (v_unsigned > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < 0) throw range_error("Value is negative");
			if (v_float > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > UINT8_MAX) throw range_error("Value too large");
			return static_cast<uint8_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::int8_t Atom::asInt8() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < INT8_MIN) throw range_error("Value too small");
			if (v_float > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > INT8_MAX) throw range_error("Value too large");
			return static_cast<int8_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::uint16_t Atom::asUInt16() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned < 0) throw range_error("Value is negative");
			if (v_unsigned > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < 0) throw range_error("Value is negative");
			if (v_float > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > UINT16_MAX) throw range_error("Value too large");
			return static_cast<uint16_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::int16_t Atom::asInt16() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < INT16_MIN) throw range_error("Value too small");
			if (v_float > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > INT16_MAX) throw range_error("Value too large");
			return static_cast<int16_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::uint32_t Atom::asUInt32() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned < 0) throw range_error("Value is negative");
			if (v_unsigned > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < 0) throw range_error("Value is negative");
			if (v_float > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > UINT32_MAX) throw range_error("Value too large");
			return static_cast<uint32_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::int32_t Atom::asInt32() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(v_signed);
		case Type::UNSIGNED:
			if (v_unsigned > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < INT32_MIN) throw range_error("Value too small");
			if (v_float > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > INT32_MAX) throw range_error("Value too large");
			return static_cast<int32_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::uint64_t Atom::asUInt64() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			if (v_unsigned < 0) throw range_error("Value is negative");
			return static_cast<uint64_t>(v_signed);
		case Type::UNSIGNED:
			return v_unsigned;
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < 0) throw range_error("Value is negative");
			if (v_float > UINT64_MAX) throw range_error("Value too large");
			return static_cast<uint64_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > UINT64_MAX) throw range_error("Value too large");
			return static_cast<uint64_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > UINT64_MAX) throw range_error("Value too large");
			return static_cast<uint64_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > UINT64_MAX) throw range_error("Value too large");
			return static_cast<uint64_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > UINT64_MAX) throw range_error("Value too large");
			return static_cast<uint64_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	std::int64_t Atom::asInt64() const {
		switch (m_type) {
		case Type::NONE:
			return 0;
		case Type::SIGNED:
			return v_signed;
		case Type::UNSIGNED:
			if (v_unsigned > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1 : 0;
		case Type::FLOAT:
			if (v_float < INT64_MIN) throw range_error("Value too small");
			if (v_float > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(v_float);
		case Type::BLOB: {
			size_t size = (v_blob == nullptr) ? 0 : v_blob->size();
			if (size > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(size);
		}
		case Type::STRING: {
			size_t size = (v_string == nullptr) ? 0 : v_string->length();
			if (size > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(size);
		}
		case Type::LIST: {
			size_t size = (v_list == nullptr) ? 0 : v_list->size();
			if (size > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(size);
		}
		case Type::OBJECT: {
			size_t size = (v_object == nullptr) ? 0 : v_object->size();
			if (size > INT64_MAX) throw range_error("Value too large");
			return static_cast<int64_t>(size);
		}
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	bool Atom::asBool() const {
		switch (m_type) {
		case Type::NONE:
			return false;
		case Type::SIGNED:
			return (v_signed != 0);
		case Type::UNSIGNED:
			return (v_unsigned != 0);
		case Type::BOOL:
			return v_bool;
		case Type::FLOAT:
			return (v_float != 0.0);
		case Type::BLOB:
			return (v_blob != nullptr);
		case Type::STRING:
			return (v_string != nullptr);
		case Type::LIST:
			return (v_list != nullptr);
		case Type::OBJECT:
			return (v_object != nullptr);
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	double Atom::asFloat() const {
		switch (m_type) {
		case Type::NONE:
			return 0.0;
		case Type::SIGNED:
			return static_cast<double>(v_signed);
		case Type::UNSIGNED:
			return static_cast<double>(v_unsigned);
		case Type::BOOL:
			return v_bool ? 1.0 : 0.0;
		case Type::FLOAT:
			return v_float;
		case Type::BLOB:
			return (v_blob == nullptr) ? 0.0 : static_cast<double>(v_blob->size());
		case Type::STRING:
			return (v_string == nullptr) ? 0.0 : static_cast<double>(v_string->length());
		case Type::LIST:
			return (v_list == nullptr) ? 0.0 : static_cast<double>(v_list->size());
		case Type::OBJECT:
			return (v_object == nullptr) ? 0.0 : static_cast<double>(v_object->size());
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	const Atom Atom::empty;

	void* Atom::operator new(size_t sz){
		return ::std::malloc(sz);
	}

	void* Atom::operator new[](size_t sz) {
		return ::operator new(sz);
	}

	Blob::Blob(const BlobValue& val) {
		m_type = Type::BLOB;
		v_blob = (val.size() > 0) ? new BlobValue(val) : nullptr;
	}

	Blob::Blob(BlobValue&& val) {
		m_type = Type::BLOB;
		v_blob = (val.size() > 0) ? new BlobValue(move(val)) : nullptr;
	}

	Blob::Blob(BlobValuePtr& ptr) {
		m_type = Type::BLOB;
		v_blob = ptr.release();
	}

	Blob::Blob(const std::uint8_t* pb, size_t cb) {
		m_type = Type::BLOB;
		if (cb != 0) {
			if (pb == nullptr) throw invalid_argument("Pointer is NULL");
			v_blob = new BlobValue(cb);
			uninitialized_copy(pb, pb + cb, &(*v_blob)[0]);
		} else {
			v_blob = nullptr;
		}
	}

	Blob::Blob(std::initializer_list<uint8_t> bytes) {
		m_type = Type::BLOB;
		if (bytes.size() != 0) {
			v_blob = new BlobValue(bytes);
		}
		else {
			v_blob = nullptr;
		}
	}

	Blob& Blob::add(std::uint8_t byte) {
		if (v_blob == nullptr) v_blob = new BlobValue();
		v_blob->push_back(byte);
		return *this;
	}

	Blob& Blob::add(std::uint8_t* pb, size_t cb) {
		if (v_blob == nullptr) v_blob = new BlobValue();
		for (; cb != 0; --cb, ++pb) v_blob->push_back(*pb);
		return *this;
	}

	Blob& Blob::add(std::initializer_list<std::uint8_t> l) {
		if (v_blob == nullptr) v_blob = new BlobValue();
		for (auto i : l) v_blob->push_back(i);
		return *this;
	}

	const BlobValue& Blob::get() const {
		return (v_blob != nullptr) ? *v_blob : emptyBlobValue;
	}

	BlobValuePtr Blob::extract() {
		return BlobValuePtr((v_blob != nullptr) ? v_blob : new BlobValue());
		v_blob = nullptr;
	}

	const Blob      Blob::empty;

	void* Blob::operator new(size_t sz){
		return ::std::malloc(sz);
	}

	void* Blob::operator new[](size_t sz) {
		return ::operator new(sz);
	}

	String::String(const std::string& val) {
		m_type = Type::STRING;
		v_string = val.empty() ? nullptr : new std::string(val);
	}

	String::String(const char* pc, size_t cc) {
		m_type = Type::STRING;
		v_string = (cc == 0) ? nullptr : new std::string(pc, cc);
	}

	String::String(std::initializer_list<char> bytes) {
		m_type = Type::STRING;
		v_string = (bytes.size() == 0) ? nullptr : new std::string(bytes);
	}

	const std::string& String::get() const {
		return (v_string == nullptr) ? emptyStringValue : *v_string;
	}

	const String      String::empty;

	void* String::operator new(size_t sz){
		return ::std::malloc(sz);
	}

	void* String::operator new[](size_t sz) {
		return ::operator new(sz);
	}

	List::List(ListValue&& val) {
		m_type = Type::LIST;
		v_list = (val.size() > 0) ? new ListValue(move(val)) : nullptr;
	}

	List::List(ListValuePtr& ptr) {
		m_type = Type::LIST;
		v_list = ptr.release();
	}

	List& List::add(ValuePtr&& ptr) {
		if (v_list == nullptr) v_list = new ListValue();
		v_list->push_back(move(ptr));
		return *this;
	}

	const ListValue& List::get() const {
		return (v_list != nullptr) ? *v_list : emptyListValue;
	}

	ListValuePtr List::extract() {
		return ListValuePtr((v_list != nullptr) ? v_list : new ListValue());
		v_list = nullptr;
	}

	const List      List::empty;

	void* List::operator new(size_t sz){
		return ::std::malloc(sz);
	}

	void* List::operator new[](size_t sz) {
		return ::operator new(sz);
	}

	Object::Object(ObjectValue&& val) {
		m_type = Type::OBJECT;
		v_object = (val.size() > 0) ? new ObjectValue(move(val)) : nullptr;
	}

	Object::Object(ObjectValuePtr& ptr) {
		m_type = Type::OBJECT;
		v_object = ptr.release();
	}

	Object& Object::add(const std::string& key, ValuePtr&& ptr) {
		if (v_object == nullptr) v_object = new ObjectValue();
		v_object->push_back(ObjectEntry{ key, move(ptr) });
		return *this;
	}

	const ObjectValue& Object::get() const {
		return (v_object != nullptr) ? *v_object : emptyObjectValue;
	}

	bool Object::contains(const std::string& key) const {
		if (v_object == nullptr) return false;
		for (auto i = v_object->begin(); i != v_object->end(); ++i) {
			if (i->first == key) return true;
		} // end for //
		return false;
	}

	const Value& Object::get(const std::string& key) const {
		if (v_object == nullptr) return Value::empty;
		for (auto i = v_object->begin(); i != v_object->end(); ++i) {
			if (i->first == key) return *i->second.get();
		} // end for //
		return Value::empty;
	}

	ObjectValuePtr Object::extract() {
		return ObjectValuePtr((v_object != nullptr) ? v_object : new ObjectValue());
		v_list = nullptr;
	}

	ValuePtr Object::extract(const std::string& key) {
		if (v_object == nullptr) return Value::make();
		for (auto i = v_object->begin(); i != v_object->end(); ++i) {
			if (i->first == key) {
				ValuePtr ptr{ i->second.release() };
				v_object->erase(i);
				return move(ptr);
			}
		} // end for //
		return Value::make();
	}

	const Object      Object::empty;

	void* Object::operator new(size_t sz){
		return ::std::malloc(sz);
	}

		void* Object::operator new[](size_t sz) {
		return ::operator new(sz);
	}

} // end namespace JsonX //
