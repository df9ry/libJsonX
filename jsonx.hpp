#ifndef JSONX_HPP
#define JSONX_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <stdint.h>

namespace jsonx {

class scanner;

class json;

typedef std::string                        json_string_t;
typedef std::vector<json>                  json_array_t;
typedef std::map<const std::string, json>  json_object_t;
typedef std::pair<const std::string, json> json_object_value_t;

class json {
public:
    // Constructors:
    json()                                          {}
    json(const json& v)                             { copy(v); }
    json(const json_array_t& v)                     { copy(v); }
    json(const json_object_t& v)                    { copy(v); }
    json(const char *v)                             { copy(v); }
    json(json&& rhs);
    json(bool v)                                    { copy(v); }
    json(int64_t v)                                 { copy(v); }
    json(int8_t v) :json(static_cast<int64_t>(v))   {}
    json(int16_t v):json(static_cast<int64_t>(v))   {}
    json(int32_t v):json(static_cast<int64_t>(v))   {}
    json(uint64_t v)                                { copy(v); }
    json(uint8_t v) :json(static_cast<uint64_t>(v)) {}
    json(uint16_t v):json(static_cast<uint64_t>(v)) {}
    json(uint32_t v):json(static_cast<uint64_t>(v)) {}
    json(double v)                                  { copy(v); }

    // Destructor:
    ~json();

    // Operations:
    void clear();
    size_t size() const;

    // IO:
    void write(std::ostream &os) const;
    json_string_t write() const         { std::ostringstream os; write(os); return os.str(); }
    void parse(std::istream &is);
    void parse(const json_string_t &s)  { std::istringstream is(s); parse(is); }

    // Type checks:
    bool isDefined()  const { return type != UNDEFINED_T; }
    bool isNull()     const { return type == NULL_T; }
    bool isBool()     const { return type == BOOL_T; }
    bool isSigned()   const { return type == SIGNED_INT_T; }
    bool isUnsigned() const { return type == UNSIGNED_INT_T; }
    bool isInteger()  const { return type == SIGNED_INT_T || type == UNSIGNED_INT_T; }
    bool isReal()     const { return type == DOUBLE_T; }
    bool isNumber()   const { return isReal() || isInteger(); }
    bool isString()   const { return type == STRING_T; }
    bool isArray()    const { return type == ARRAY_T; }
    bool isObject()   const { return type == OBJECT_T; }

    // Constants:
    static const json     undefined;
    static const json     null;

    // Type conversions:
    bool                 toBool() const;
    bool&                toBoolRef();
    int64_t              toSigned() const;
    int64_t&             toSignedRef64();
    int32_t&             toSignedRef32();
    int16_t&             toSignedRef16();
    int8_t&              toSignedRef8();
    uint64_t             toUnsigned() const;
    uint64_t&            toUnsignedRef64();
    uint32_t&            toUnsignedRef32();
    uint16_t&            toUnsignedRef16();
    uint8_t&             toUnsignedRef8();
    int                  toInteger() const{
        return static_cast<int>(toSigned());
    }
    double               toReal() const;
    double&              toRealRef();
    double               toNumber() {
        return toReal();
    }
    const json_string_t  toString() const;
          json_string_t  toString();
    const json_string_t& toStringRef() const;
          json_string_t& toStringRef();
    const json_array_t&  toArray() const;
          json_array_t   toArray();
          json_array_t&  toArrayRef();
    const json_object_t& toObject() const;
          json_object_t  toObject();
          json_object_t& toObjectRef();

    // Automatic type conversations:
    operator bool() const {
        return toBool();
    }
    operator bool&() {
        return toBoolRef();
    }
    operator int64_t() const {
        return toSigned();
    }
    operator int32_t() const {
        return static_cast<int32_t>(toSigned());
    }
    operator int16_t() const {
        return static_cast<int16_t>(toSigned());
    }
    operator int8_t() const {
        return static_cast<int8_t>(toSigned());
    }
    operator int64_t&() {
        return toSignedRef64();
    }
    operator int32_t&() {
        return toSignedRef32();
    }
    operator int16_t&() {
        return toSignedRef16();
    }
    operator int8_t&() {
        return toSignedRef8();
    }
    operator uint64_t() const {
        return toUnsigned();
    }
    operator uint32_t() const {
        return static_cast<uint32_t>(toUnsigned());
    }
    operator uint16_t() const {
        return static_cast<uint16_t>(toUnsigned());
    }
    operator uint8_t() const {
        return static_cast<uint8_t>(toUnsigned());
    }
    operator uint64_t&() {
        return toUnsignedRef64();
    }
    operator uint32_t&() {
        return toUnsignedRef32();
    }
    operator uint16_t&() {
        return toUnsignedRef16();
    }
    operator uint8_t&() {
        return toUnsignedRef8();
    }
    operator double() const {
        return toReal();
    }
    operator double&() {
        return toRealRef();
    }
    operator const json_string_t() const {
        return toString();
    }
    operator json_string_t&() {
        return toStringRef();
    }

    // JSON value:
    void setUndefined()              { clear(); }
    void setNull();
    void set(const json& v)          { clear(); copy(v); }
    void set(bool v)                 { clear(); copy(v); }
    void set(int8_t v)               { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint8_t v)              { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int16_t v)              { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint16_t v)             { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int32_t v)              { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint32_t v)             { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int64_t v)              { clear(); copy(v); }
    void set(uint64_t v)             { clear(); copy(v); }
    void set(double v)               { clear(); copy(v); }
    void set(const json_string_t& v) { clear(); copy(v.c_str()); }
    void set(const char *v)          { clear(); copy(v); }
    void set(const json_array_t& v)  { clear(); copy(v); }
    void set(const json_object_t& v) { clear(); copy(v); }

    // Assignments:
    void operator=(json& v)                { set(v); }
    void operator=(bool v)                 { set(v); }
    void operator=(int8_t v)               { set(v); }
    void operator=(uint8_t v)              { set(v); }
    void operator=(int16_t v)              { set(v); }
    void operator=(uint16_t v)             { set(v); }
    void operator=(int32_t v)              { set(v); }
    void operator=(uint32_t v)             { set(v); }
    void operator=(int64_t v)              { set(v); }
    void operator=(uint64_t v)             { set(v); }
    void operator=(double v)               { set(v); }
    void operator=(const json_string_t &v) { set(v); }
    void operator=(const char *v)          { set(json_string_t(v)); }
    void operator=(const json_array_t& v)  { set(v); }
    void operator=(const json_object_t& v) { set(v); }

    // Compares:
    bool operator==(const char *v) const { return operator==(json(v)); }
    bool operator==(const json &v) const;
    bool operator!=(const json &v) const { return !operator==(v); }
    bool operator<(const json &v) const;
    bool operator<=(const json &v) const { return operator<(v) || operator==(v); }
    bool operator>(const json &v) const { return !operator<=(v); }
    bool operator>=(const json &v) const { return !operator<(v); }
    bool operator==(const json_array_t &v) const;
    bool operator==(const json_object_t &v) const;

    // JSON array:
    void add(const json &j);
    void addUndefined()              { add(undefined); }
    void addNull()                   { add(null);      }
    void add(bool v)                 { add(json(v));   }
    void add(int64_t v)              { add(json(v));   }
    void add(uint64_t v)             { add(json(v));   }
    void add(double v)               { add(json(v));   }
    void add(const json_string_t& v) { add(json(v.c_str())); }
    void add(const char *v)          { add(json(v)); }
    void add(const json_array_t& v)  { add(json(v));   }
    void add(const json_object_t& v) { add(json(v));   }

    // JSON object:
    void add(const std::string &key, const json &j);
    void addUndefined(const std::string &key)                { add(key, undefined); }
    void addNull(const std::string &key)                     { add(key, null);      }
    void add(const std::string &key, bool v)                 { add(key, json(v));   }
    void add(const std::string &key, int64_t v)              { add(key, json(v));   }
    void add(const std::string &key, uint64_t v)             { add(key, json(v));   }
    void add(const std::string &key, double v)               { add(key, json(v));   }
    void add(const std::string &key, const json_string_t& v) { add(key, json(v.c_str()));   }
    void add(const std::string &key, const char *v)          { add(key, json(v)); }
    void add(const std::string &key, const json_array_t& v)  { add(key, json(v));   }
    void add(const std::string &key, const json_object_t& v) { add(key, json(v));   }

    // Subscriptions:
    json& at(int i);
    json& operator[] (int i)           { return at(i); }
    json& find(const char *key);
    json& operator[] (const char *key) { return find(key); }

private:
    typedef enum {
        UNDEFINED_T,
        NULL_T,
        BOOL_T,
        SIGNED_INT_T,
        UNSIGNED_INT_T,
        DOUBLE_T,
        STRING_T,
        ARRAY_T,
        OBJECT_T
    } DataType;

    explicit json(DataType t);

    void copy(const json& v);
    void copy(bool v);
    void copy(int64_t v);
    void copy(uint64_t v);
    void copy(double v);
    void copy(const char *v);
    void copy(const json_array_t& v);
    void copy(const json_object_t& v);

    void parse(scanner &sc);

    friend class json_ref;
    friend class json_const;

    friend std::ostream &operator<<(std::ostream &os, const json &j)
    {
        j.write(os);
        return os;
    }

    friend std::istream &operator>>(std::istream &is, json &j)
    {
        j.parse(is);
        return is;
    }

    DataType type{UNDEFINED_T};
    union {
        bool            bool_value;
        uint64_t        uint_value;
        int64_t         int_value;
        double          real_value;
        json_string_t  *string_value;
        json_array_t   *array_value;
        json_object_t  *object_value;
    };
}; // end class json //

// Json object value helper:
inline json_object_value_t jitem(const char *key, const json& val)
{
    return json_object_value_t(key, val);
}

json jarray(std::initializer_list<json> v);

json jobject(std::initializer_list<json_object_value_t> v);

} // end namespace jsonx //

#endif // JSONX_HPP
