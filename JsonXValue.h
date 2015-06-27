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

#include "JsonXType.h"

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <initializer_list>

#ifdef LIBJSONX_EXPORTS
# ifdef _WIN32
#  define LIBJSONX_EXPORT __declspec(dllexport)
# else
#  define LIBJSONX_EXPORT __attribute__ ((dllexport))
# endif
#else
# define LIBJSONX_EXPORT
#endif

/**
 * All JsonX related functionality is enclosed in this namespace.
 */
namespace JsonX {

	class Value;
	class Blob;
	class String;
	class Atom;
	class Object;
	class List;

	/**
	 * Return string representation of a type.
	 * @param t The JsonX type.
	 * @return String representation of t.
	 */
	LIBJSONX_EXPORT extern const std::string TypeString(Type t);

	/**
	 * Unique pointer to JsonX value.
	 */
	using ValuePtr       = std::unique_ptr < Value >;

	/**
	 * Unique pointer to JsonX atom.
	 */
	using AtomPtr        = std::unique_ptr < Atom >;

	/**
	 * Unique pointer to JsonX blob.
	 */
	using BlobPtr        = std::unique_ptr < Blob >;

	/**
	 * Unique pointer to JsonX string.
	 */
	using StringPtr      = std::unique_ptr < String >;

	/**
	 * Unique pointer to JsonX list.
	 */
	using ListPtr        = std::unique_ptr < List >;

	/**
	 * Unique pointer to JsonX object.
	 */
	using ObjectPtr      = std::unique_ptr < Object >;

	/**
	 * Value of a JsonX blob.
	 */
	using BlobValue      = std::vector < uint8_t >;

	/**
	 * Unique pointer to value of a JsonX blob.
	 */
	using BlobValuePtr   = std::unique_ptr < BlobValue >;

	/**
	 * Value of a JsonX list.
	 */
	using ListValue      = std::vector < ValuePtr >;

	/**
	 * Unique pointer to value of a JsonX list.
	 */
	using ListValuePtr   = std::unique_ptr < ListValue >;

	/**
	 * Entry of a JsonX object.
	 */
	using ObjectEntry    = std::pair < std::string, ValuePtr >;

	/**
	 * Value of a JsonX object.
	 */
	using ObjectValue    = std::vector < ObjectEntry >;

	/**
	 * Unique pointer to value of a JsonX object.
	 */
	using ObjectValuePtr = std::unique_ptr < ObjectValue >;

	/**
	 * Base class for all JsonX values.
	 */
	class LIBJSONX_EXPORT Value {
	public:
		/**
		 * Get type of this value.
		 * @return Type of this value.
		 */
		Type type() const { return m_type; }

		/**
		 * Test if this value is empty.
		 * @return True, if this value is empty.
		 */
		bool isEmpty()  const;

		/**
		 * Test if this value is an atom.
		 * @return True, if this value is an atom.
		 */
		bool isAtom()   const { return m_type < Type::BLOB;    }

		/**
		 * Test if this value is a blob.
		 * @return True, if this value is a blob.
		 */
		bool isBlob()   const { return m_type == Type::BLOB;   }

		/**
		 * Test if this value is a string.
		 * @return True, if this value is a string.
		 */
		bool isString() const { return m_type == Type::STRING; }

		/**
		 * Test if this value is a list.
		 * @return True, if this value is a list.
		 */
		bool isList()   const { return m_type == Type::LIST;   }

		/**
		 * Test if this value is an object.
		 * @return True, if this value is am object.
		 */
		bool isObject() const { return m_type == Type::OBJECT; }

		/**
		 * Convert this value to an atom.
		 * @return This value as an atom.
		 * @throw Exception, if this value is not an atom.
		 */
		Atom&   asAtom();

		/**
		 * Convert this value to a blob.
		 * @return This value as a blob.
		 * @throw Exception, if this value is not a blob.
		 */
		Blob&   asBlob();

		/**
		 * Convert this value to a string.
		 * @return This value as a string.
		 * @throw Exception, if this value is not a string.
		 */
		String& asString();

		/**
		 * Convert this value to a list.
		 * @return This value as a list.
		 * @throw Exception, if this value is not a list.
		 */
		List&   asList();

		/**
		 * Convert this value to a object.
		 * @return This value as a object.
		 * @throw Exception, if this value is not a object.
		 */
		Object& asObject();

		/**
		 * Convert this value to a const atom.
		 * @return This value as a const atom.
		 * @throw Exception, if this value is not an atom.
		 */
		const Atom&   asConstAtom()   const;

		/**
		 * Convert this value to a const blob.
		 * @return This value as a const blob.
		 * @throw Exception, if this value is not a blob.
		 */
		const Blob&   asConstBlob()   const;

		/**
		 * Convert this value to a const string.
		 * @return This value as a const string.
		 * @throw Exception, if this value is not a string.
		 */
		const String& asConstString() const;

		/**
		 * Convert this value to a const list.
		 * @return This value as a const list.
		 * @throw Exception, if this value is not a list.
		 */
		const List&   asConstList()   const;

		/**
		 * Convert this value to a const object.
		 * @return This value as a const object.
		 * @throw Exception, if this value is not an object.
		 */
		const Object& asConstObject() const;

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		Value(const Value& other) = delete;

		/**
		 * Move constructor.
		 * @param other The value to move from.
		 */
		Value(Value&& other);

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		Value& operator=(const Value& other) = delete;

		/**
		 * Move assignment.
		 * @param other The move source.
		 * @return Move target.
		 */
		Value& operator=(Value&& other);

		/**
		 * Set value to empty, release all resources.
		 */
		void reset();

		/**
		 * Destructor.
		 */
		~Value() { reset(); }

		/**
		 * Get string representation of this value.
		 * @return String representation of this value.
		 */
		std::string toJsonString() const;

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Construct JsonX tree from string representation.
		 * @param s The JsonX string.
		 * @return JsonX tree.
		 */
		static ValuePtr fromJson(const std::string& s);

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static ValuePtr fromJson(std::istream& is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static ValuePtr make() { return ValuePtr{ new Value() }; }

		/**
		 * Constant empty value.
		 */
		static const Value empty;

	protected:
		Value()                : m_type(Type::NONE)                    {}
		Value(std::int64_t  v) : m_type(Type::SIGNED), v_signed(v)     {}
		Value(std::uint64_t v) : m_type(Type::UNSIGNED), v_unsigned(v) {}
		Value(bool          v) : m_type(Type::BOOL), v_bool(v)         {}
		Value(double        v) : m_type(Type::FLOAT), v_float(v)       {}
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects

		Type  m_type{ Type::NONE };
		union {
			std::int64_t  v_signed;
			std::uint64_t v_unsigned;
			bool          v_bool;
			double        v_float;
			BlobValue*    v_blob;
			std::string*  v_string;
			ListValue*    v_list;
			ObjectValue*  v_object;
		};
	};

	/**
	 * Represent a JsonX atom.
	 */
	class LIBJSONX_EXPORT Atom : public Value {
	public:
		/**
		 * Standard constructor.
		 */
		Atom(               ) : Value( ) {}

		/**
		 * Constructor with initial value.
		 * @param v Initial value.
		 */
		Atom(std::int64_t  v) : Value(v) {}

		/**
		 * Constructor with initial value.
		 * @param v Initial value.
		 */
		Atom(std::uint64_t v) : Value(v) {}

		/**
		 * Constructor with initial value.
		 * @param v Initial value.
		 */
		Atom(bool          v) : Value(v) {}

		/**
		 * Constructor with initial value.
		 * @param v Initial value.
		 */
		Atom(double        v) : Value(v) {}

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		Atom(const Atom& other) = delete;

		/**
		 * Move constructor.
		 * @param other Source value.
		 */
		Atom(Atom&& other) : Value(std::move(other)) {};

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		Atom& operator=(const Atom& other) = delete;

		/**
		 * Move assignment.
		 * @param other The source value.
		 * @return Target value.
		 */
		Atom& operator=(Atom&& other) {
			return static_cast<Atom&>(Value::operator=(std::move(other)));
		}

		/**
		 * Get value as 8 bit unsigned integer.
		 * @return Value as 8 bit unsigned integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::uint8_t asUInt8() const;

		/**
		 * Get value as 8 bit signed integer.
		 * @return Value as 8 bit signed integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::int8_t asInt8() const;

		/**
		 * Get value as 16 bit unsigned integer.
		 * @return Value as 16 bit unsigned integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::uint16_t asUInt16() const;

		/**
		 * Get value as 16 bit signed integer.
		 * @return Value as 16 bit signed integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::int16_t asInt16() const;

		/**
		 * Get value as 32 bit unsigned integer.
		 * @return Value as 32 bit unsigned integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::uint32_t asUInt32() const;

		/**
		 * Get value as 32 bit signed integer.
		 * @return Value as 32 bit signed integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::int32_t asInt32() const;

		/**
		 * Get value as 64 bit unsigned integer.
		 * @return Value as 64 bit unsigned integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::uint64_t asUInt64() const;

		/**
		 * Get value as 64 bit signed integer.
		 * @return Value as 64 bit signed integer.
		 * @throw Exception if value doesn't fit in.
		 */
		std::int64_t asInt64() const;

		/**
		 * Get value as boolean.
		 * @return Value as boolean.
		 * @throw Exception if value cannot converted to bool.
		 */
		bool asBool() const;

		/**
		 * Get value as double precision floating point.
		 * @return Value as double precision floating point.
		 */
		double asFloat() const;

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static AtomPtr fromJson(std::istream& is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static AtomPtr make() { return AtomPtr{ new Atom() }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static AtomPtr make(std::uint64_t val) { return AtomPtr{ new Atom(val) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static AtomPtr make(std::int64_t  val) { return AtomPtr{ new Atom(val) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static AtomPtr make(bool          val) { return AtomPtr{ new Atom(val) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static AtomPtr make(double        val) { return AtomPtr{ new Atom(val) }; }

		/**
		 * Constant empty value.
		 */
		static const Atom empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	/**
	 * Represent a JsonX blob.
	 */
	class LIBJSONX_EXPORT Blob : public Value {
	public:
		/**
		 * Standard constructor.
		 */
		Blob() : Value() { m_type = Type::BLOB; v_blob = nullptr; }

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		Blob(const Blob& other) = delete;

		/**
		 * Move constructor.
		 * @param other Source value.
		 */
		Blob(Blob&& other) : Value(std::move(other)) {};

		/**
		 * Construct with copy of initial value.
		 * @param val Initial value.
		 */
		Blob(const BlobValue& val);

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		Blob(BlobValue&& val);

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		Blob(BlobValuePtr& ptr);

		/**
		 * Construct with copy of raw data array.
		 * @param pb Pointer to array.
		 * @param cb Size of array.
		 */
		Blob(const std::uint8_t* pb, size_t cb);

		/**
		 * Construct with initializer list.
		 * @param bytes Initial value.
		 */
		Blob(std::initializer_list<uint8_t> bytes);

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static BlobPtr fromJson(std::istream& is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static BlobPtr make() { return BlobPtr{ new Blob() }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static BlobPtr make(const BlobValue& val                 ) { return BlobPtr{ new Blob(val      ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static BlobPtr make(BlobValue&& val                      ) { return BlobPtr{ new Blob(move(val)) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param ptr Initial value.
		 * @return Unique pointer with initial value.
		 */
		static BlobPtr make(BlobValuePtr& ptr                    ) { return BlobPtr{ new Blob(ptr      ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param pb Pointer to initial value.
		 * @param cb Size of initial value.
		 * @return Unique pointer with initial value.
		 */
		static BlobPtr make(const std::uint8_t* pb, size_t cb    ) { return BlobPtr{ new Blob(pb, cb   ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param l Initial value.
		 * @return Unique pointer with initial value.
		 */
		static BlobPtr make(std::initializer_list<std::uint8_t> l) { return BlobPtr{ new Blob(l        ) }; }

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		Blob& operator=(const Blob& other) = delete;

		/**
		 * Move assignment.
		 * @param other The source value.
		 * @return Target value.
		 */
		Blob& operator=(Blob&& other) {
			return static_cast<Blob&>(Value::operator=(std::move(other)));
		}

		/**
		 * Add single byte to the blob.
		 * @param byte Byte to add.
		 * @return Reference to self.
		 */
		Blob& add(std::uint8_t byte);

		/**
		 * Add raw array to blob.
		 * @param pb Pointer to the raw array.
		 * @param cb Size of the raw array.
		 * @return Reference to self.
		 */
		Blob& add(std::uint8_t* pb, size_t cb);

		/**
		 * Add bytes to blob.
		 * @param l Bytes to append.
		 * @return Reference to self.
		 */
		Blob& add(std::initializer_list<std::uint8_t> l);

		/**
		 * Get const reference to the value.
		 * @return Const reference to the value.
		 */
		const BlobValue& get() const;

		/**
		 * Extract the value.
		 * @return Extracted value.
		 */
		BlobValuePtr extract();

		/**
		 * Constant empty value.
		 */
		static const Blob      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	/**
	 * Represent a JsonX string.
	 */
	class LIBJSONX_EXPORT String : public Value {
	public:
		/**
		 * Standard constructor.
		 */
		String() : Value() { m_type = Type::STRING; v_string = nullptr; }

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		String(const String& other) = delete;

		/**
		 * Move constructor.
		 * @param other Source value.
		 */
		String(String&& other) : Value(std::move(other)) {};

		/**
		 * Construct with copy of initial value.
		 * @param val Initial value.
		 */
		String(const std::string& val);

		/**
		 * Construct with copy of a raw array.
		 * @param pc Pointer to raw array.
		 * @param cc Size of the raw array.
		 */
		String(const char* pc, size_t cc);

		/**
		 * Construct with initializer list.
		 * @param chars Characters to add.
		 */
		String(std::initializer_list<char> chars);

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Encode a raw string JsonX like.
		 * @param os Stream to write to.
		 * @param s Raw string to encode.
		 */
		static void toJson(std::ostream& os, const std::string& s);

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static StringPtr fromJson(std::istream& is);

		/**
		 * Construct raw string from text representation.
		 * @param is Stream to read from.
		 * @return Raw string.
		 */
		static std::string fromJsonRaw(std::istream&is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static StringPtr make() { return StringPtr{ new String() }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static StringPtr make(const std::string& val       ) { return StringPtr{ new String(val      ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param pc Pointer to initial value.
		 * @param cc Size of initial value.
		 * @return Unique pointer with initial value.
		 */
		static StringPtr make(const char* pc, size_t cc    ) { return StringPtr{ new String(pc, cc   ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param l Initial value.
		 * @return Unique pointer with initial value.
		 */
		static StringPtr make(std::initializer_list<char> l) { return StringPtr{ new String(l        ) }; }

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		String& operator=(const String& other) = delete;

		/**
		 * Move assignment.
		 * @param other The source value.
		 * @return Target value.
		 */
		String& operator=(String&& other) {
			return static_cast<String&>(Value::operator=(std::move(other)));
		}

		/**
		 * Get constant reference to value.
		 * @return Constant reference to value.
		 */
		const std::string& get() const;

		/**
		 * Constant empty value.
		 */
		static const String      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	/**
	 * Represent a JsonX list.
	 */
	class LIBJSONX_EXPORT List : public Value {
	public:
		/**
		 * Standard constructor.
		 */
		List() : Value() { m_type = Type::LIST; v_list = nullptr; }

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		List(const List& other) = delete;

		/**
		 * Move constructor.
		 * @param other Source value.
		 */
		List(List&& other) : Value(std::move(other)) {};

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		List(ListValue&& val);

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		List(ListValuePtr& ptr);

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static ListPtr fromJson(std::istream& is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static ListPtr make() { return ListPtr{ new List() }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static ListPtr make(ListValue&& val) { return ListPtr{ new List(move(val)) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param ptr Initial value.
		 * @return Unique pointer with initial value.
		 */
		static ListPtr make(ListValuePtr& ptr) { return ListPtr{ new List(ptr) }; }

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		List& operator=(const List& other) = delete;

		/**
		 * Move assignment.
		 * @param other The source value.
		 * @return Target value.
		 */
		List& operator=(List&& other) {
			return static_cast<List&>(Value::operator=(std::move(other)));
		}

		/**
		 * Add a value with move.
		 * @param ptr Pointer to source value.
		 * @return Reference to self.
		 */
		List& add(ValuePtr&& ptr);

		/**
		 * Get constant reference to value.
		 * @return Constant reference to value.
		 */
		const ListValue& get() const;

		/**
		 * Extract value.
		 * @return Extracted value.
		 */
		ListValuePtr extract();

		/**
		 * Constant empty value.
		 */
		static const List      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	/**
	 * Represent a JsonX object.
	 */
	class LIBJSONX_EXPORT Object : public Value {
	public:
		/**
		 * Standard constructor.
		 */
		Object() : Value() { m_type = Type::OBJECT; v_object = nullptr; }

		/**
		 * Copy construction is not allowed.
		 * @param other
		 */
		Object(const Object& other) = delete;

		/**
		 * Move constructor.
		 * @param other Source value.
		 */
		Object(Object&& other) : Value(std::move(other)) {};

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		Object(ObjectValue&& val);

		/**
		 * Construct with move of initial value.
		 * @param val Initial value.
		 */
		Object(ObjectValuePtr& ptr);

		/**
		 * Get textual representation of this value.
		 * @param os Stream to write to.
		 */
		void toJson(std::ostream& os) const;

		/**
		 * Construct JsonX tree from text representation.
		 * @param is Stream to read from.
		 * @return JsonX tree.
		 */
		static ObjectPtr fromJson(std::istream& is);

		/**
		 * Make new unique pointer to a value.
		 * @return Unique pointer to new value.
		 */
		static ObjectPtr make(                   ) { return ObjectPtr{ new Object(         ) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param val Initial value.
		 * @return Unique pointer with initial value.
		 */
		static ObjectPtr make(ObjectValue&& val  ) { return ObjectPtr{ new Object(move(val)) }; }

		/**
		 * Make new unique pointer with initial value.
		 * @param ptr Initial value.
		 * @return Unique pointer with initial value.
		 */
		static ObjectPtr make(ObjectValuePtr& ptr) { return ObjectPtr{ new Object(ptr      ) }; }

		/**
		 * Copy assignment is not allowed.
		 * @param other
		 * @return
		 */
		Object& operator=(const Object& other) = delete;

		/**
		 * Move assignment.
		 * @param other The source value.
		 * @return Target value.
		 */
		Object& operator=(Object&& other) {
			return static_cast<Object&>(Value::operator=(std::move(other)));
		}

		/**
		 * Move value into the object.
		 * @param key Key of the value.
		 * @param ptr Pointer to the value.
		 * @return Reference to self.
		 */
		Object& add(const std::string& key, ValuePtr&& ptr);

		/**
		 * Test if the object contains a key.
		 * @param key The key to test.
		 * @return True, if key is contained in the object.
		 */
		bool contains(const std::string& key) const;

		/**
		 * Get constant reference to value.
		 * @return Constant reference to value.
		 */
		const ObjectValue& get() const;

		/**
		 * Get constant reference to value.
		 * @param key The key to lookup value.
		 * @return Constant reference to value.
		 */
		const Value& get(const std::string& key) const;

		/**
		 * Extract value.
		 * @return Extracted value.
		 */
		ObjectValuePtr extract();

		/**
		 * Extract value.
		 * @param key The key to lookup value.
		 * @return Extracted value.
		 */
		ValuePtr extract(const std::string& key);

		/**
		 * Constant empty value.
		 */
		static const Object      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

} // end namespace JsonX //

#endif
