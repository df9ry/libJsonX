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

namespace JsonX {

	class Value;
	class Blob;
	class String;
	class Atom;
	class Object;
	class List;

	LIBJSONX_EXPORT extern const std::string TypeString(Type t);

	using ValuePtr       = std::unique_ptr < Value >;
	using ValuePtr       = std::unique_ptr < Value >;
	using AtomPtr        = std::unique_ptr < Atom >;
	using BlobPtr        = std::unique_ptr < Blob >;
	using StringPtr      = std::unique_ptr < String >;
	using ListPtr        = std::unique_ptr < List >;
	using ObjectPtr      = std::unique_ptr < Object >;
	using BlobValue      = std::vector < uint8_t >;
	using BlobValuePtr   = std::unique_ptr < BlobValue >;
	using ListValue      = std::vector < ValuePtr >;
	using ListValuePtr   = std::unique_ptr < ListValue >;
	using ObjectEntry    = std::pair < std::string, ValuePtr >;
	using ObjectValue    = std::vector < ObjectEntry >;
	using ObjectValuePtr = std::unique_ptr < ObjectValue >;

	class LIBJSONX_EXPORT Value {
	public:
		Type type() const { return m_type; }
		bool isEmpty()  const;
		bool isAtom()   const { return m_type < Type::BLOB;    }
		bool isBlob()   const { return m_type == Type::BLOB;   }
		bool isString() const { return m_type == Type::STRING; }
		bool isList()   const { return m_type == Type::LIST;   }
		bool isObject() const { return m_type == Type::OBJECT; }

		Atom&   asAtom();
		Blob&   asBlob();
		String& asString();
		List&   asList();
		Object& asObject();

		const Atom&   asConstAtom()   const;
		const Blob&   asConstBlob()   const;
		const String& asConstString() const;
		const List&   asConstList()   const;
		const Object& asConstObject() const;

		void reset();

		Value(const Value& other) = delete;
		Value(Value&& other);
		Value& operator=(const Value& other) = delete;
		Value& operator=(Value&& other);
		~Value() { reset(); }

		std::string toJson() const;
		void toJson(std::ostream& os) const;

		static ValuePtr fromJson(const std::string& s);
		static ValuePtr fromJson(std::istream& is);

		static ValuePtr make() { return ValuePtr{ new Value() }; }

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

	class LIBJSONX_EXPORT Atom : public Value {
	public:
		Atom(               ) : Value( ) {}
		Atom(std::int64_t  v) : Value(v) {}
		Atom(std::uint64_t v) : Value(v) {}
		Atom(bool          v) : Value(v) {}
		Atom(double        v) : Value(v) {}

		Atom(const Atom& other) = delete;
		Atom(Atom&& other) : Value(std::move(other)) {};

		Atom& operator=(const Atom& other) = delete;
		Atom& operator=(Atom&& other) {
			return static_cast<Atom&>(Value::operator=(std::move(other)));
		}

		std::uint8_t asUInt8() const;
		std::int8_t asInt8() const;
		std::uint16_t asUInt16() const;
		std::int16_t asInt16() const;
		std::uint32_t asUInt32() const;
		std::int32_t asInt32() const;
		std::uint64_t asUInt64() const;
		std::int64_t asInt64() const;
		bool asBool() const;
		double asFloat() const;

		void toJson(std::ostream& os) const;
		static AtomPtr fromJson(std::istream& is);

		static AtomPtr make() { return AtomPtr{ new Atom() }; }
		static AtomPtr make(std::uint64_t val) { return AtomPtr{ new Atom(val) }; }
		static AtomPtr make(std::int64_t  val) { return AtomPtr{ new Atom(val) }; }
		static AtomPtr make(bool          val) { return AtomPtr{ new Atom(val) }; }
		static AtomPtr make(double        val) { return AtomPtr{ new Atom(val) }; }

		static const Atom empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	class LIBJSONX_EXPORT Blob : public Value {
	public:
		Blob() : Value() { m_type = Type::BLOB; v_blob = nullptr; }

		Blob(const Blob& other) = delete;
		Blob(Blob&& other) : Value(std::move(other)) {};
		Blob(const BlobValue& val);
		Blob(BlobValue&& val);
		Blob(BlobValuePtr& ptr);
		Blob(const std::uint8_t* pb, size_t cb);
		Blob(std::initializer_list<uint8_t> bytes);

		void toJson(std::ostream& os) const;
		static BlobPtr fromJson(std::istream& is);

		static BlobPtr make() { return BlobPtr{ new Blob() }; }
		static BlobPtr make(const BlobValue& val                 ) { return BlobPtr{ new Blob(val      ) }; }
		static BlobPtr make(BlobValue&& val                      ) { return BlobPtr{ new Blob(move(val)) }; }
		static BlobPtr make(BlobValuePtr& ptr                    ) { return BlobPtr{ new Blob(ptr      ) }; }
		static BlobPtr make(const std::uint8_t* pb, size_t cb    ) { return BlobPtr{ new Blob(pb, cb   ) }; }
		static BlobPtr make(std::initializer_list<std::uint8_t> l) { return BlobPtr{ new Blob(l        ) }; }

		Blob& operator=(const Blob& other) = delete;
		Blob& operator=(Blob&& other) {
			return static_cast<Blob&>(Value::operator=(std::move(other)));
		}

		Blob& add(std::uint8_t byte);
		Blob& add(std::uint8_t* pb, size_t cb);
		Blob& add(std::initializer_list<std::uint8_t> l);

		const BlobValue& get() const;
		BlobValuePtr extract();

		static const Blob      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	class LIBJSONX_EXPORT String : public Value {
	public:
		String() : Value() { m_type = Type::STRING; v_string = nullptr; }

		String(const String& other) = delete;
		String(String&& other) : Value(std::move(other)) {};
		String(const std::string& val);
		String(const char* pc, size_t cc);
		String(std::initializer_list<char> bytes);

		void toJson(std::ostream& os) const;
		static void toJson(std::ostream& os, const std::string& s);
		static StringPtr fromJson(std::istream& is);
		static std::string fromJsonRaw(std::istream&is);

		static StringPtr make() { return StringPtr{ new String() }; }
		static StringPtr make(const std::string& val       ) { return StringPtr{ new String(val      ) }; }
		static StringPtr make(const char* pc, size_t cc    ) { return StringPtr{ new String(pc, cc   ) }; }
		static StringPtr make(std::initializer_list<char> l) { return StringPtr{ new String(l        ) }; }

		String& operator=(const String& other) = delete;
		String& operator=(String&& other) {
			return static_cast<String&>(Value::operator=(std::move(other)));
		}

		const std::string& get() const;

		static const String      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	class LIBJSONX_EXPORT List : public Value {
	public:
		List() : Value() { m_type = Type::LIST; v_list = nullptr; }

		List(const List& other) = delete;
		List(List&& other) : Value(std::move(other)) {};
		List(ListValue&& val);
		List(ListValuePtr& ptr);

		void toJson(std::ostream& os) const;
		static ListPtr fromJson(std::istream& is);

		static ListPtr make() { return ListPtr{ new List() }; }
		static ListPtr make(ListValue&& val) { return ListPtr{ new List(move(val)) }; }
		static ListPtr make(ListValuePtr& ptr) { return ListPtr{ new List(ptr) }; }

		List& operator=(const List& other) = delete;
		List& operator=(List&& other) {
			return static_cast<List&>(Value::operator=(std::move(other)));
		}

		List& add(ValuePtr&& ptr);

		const ListValue& get() const;
		ListValuePtr extract();

		static const List      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

	class LIBJSONX_EXPORT Object : public Value {
	public:
		Object() : Value() { m_type = Type::OBJECT; v_object = nullptr; }

		Object(const Object& other) = delete;
		Object(Object&& other) : Value(std::move(other)) {};
		Object(ObjectValue&& val);
		Object(ObjectValuePtr& ptr);

		void toJson(std::ostream& os) const;
		static ObjectPtr fromJson(std::istream& is);

		static ObjectPtr make(                   ) { return ObjectPtr{ new Object(         ) }; }
		static ObjectPtr make(ObjectValue&& val  ) { return ObjectPtr{ new Object(move(val)) }; }
		static ObjectPtr make(ObjectValuePtr& ptr) { return ObjectPtr{ new Object(ptr      ) }; }

		Object& operator=(const Object& other) = delete;
		Object& operator=(Object&& other) {
			return static_cast<Object&>(Value::operator=(std::move(other)));
		}

		Object& add(const std::string& key, ValuePtr&& ptr);

		bool contains(const std::string& key) const;

		const ObjectValue& get() const;
		const Value& get(const std::string& key) const;
		ObjectValuePtr extract();
		ValuePtr extract(const std::string& key);

		static const Object      empty;

	private:
		static void* operator new(size_t);    // To prevent allocation of scalar objects
		static void* operator new[](size_t);  // To prevent allocation of array of objects
	};

} // end namespace JsonX //

#endif
