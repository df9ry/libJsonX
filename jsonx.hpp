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

typedef enum {
    UNDEFINED,
    NIL,
    BOOLEAN,
    SIGNED_INT,
    UNSIGNED_INT,
    DOUBLE,
    STRING,
    ARRAY,
    OBJECT
} DataType;

class json {
public:
    json() {}
    json(const json& v)                        { copy(v); }
    json(json&& rhs);
    json(bool v)                               { copy(v); }
    json(int64_t v)                            { copy(v); }
    json(int8_t v) :json(static_cast<int64_t>(v)) {}
    json(int16_t v):json(static_cast<int64_t>(v)) {}
    json(int32_t v):json(static_cast<int64_t>(v)) {}
    json(uint64_t v)                           { copy(v); }
    json(uint8_t v) :json(static_cast<uint64_t>(v)) {}
    json(uint16_t v):json(static_cast<uint64_t>(v)) {}
    json(uint32_t v):json(static_cast<uint64_t>(v)) {}
    json(double v)                             { copy(v); }
    json(const std::string& v)                 { copy(v); }
    json(const char *v):json(std::string(v)) {}
    json(const std::vector<json>& v)           { copy(v); }
    json(const std::map<std::string, json>& v) { copy(v); }
    ~json();

    bool isDefined()  const { return type != UNDEFINED;    }
    bool isNil()      const { return type == NIL;          }
    bool isBool()     const { return type == BOOLEAN;      }
    bool isSigned()   const { return type == SIGNED_INT;   }
    bool isUnsigned() const { return type == UNSIGNED_INT; }
    bool isInteger()  const { return type == SIGNED_INT || type == UNSIGNED_INT; }
    bool isReal()     const { return type == DOUBLE;       }
    bool isNumber()   const { return isReal() || isInteger(); }
    bool isString()   const { return type == STRING;       }
    bool isArray()    const { return type == ARRAY;        }
    bool isObject()   const { return type == OBJECT;       }

    static json _undefined() { return json(); }
    static json _null() { json j; j.type = NIL; return j; }

    bool                               toBool()     const;
    int64_t                            toSigned()   const;
    uint64_t                           toUnsigned() const;
    int64_t                            toInteger()  const { return toSigned(); }
    double                             toReal()     const;
    double                             toNumber()   const { return toReal();     }
    const std::string                  toString()   const;
    const std::vector<json>            toArray()    const;
    const std::map<std::string, json>  toObject()   const;

    // JSON value:
    void setUndefined() { clear(); }
    void setNil();
    void set(const json& v)                        { clear(); copy(v); }
    void set(bool v)                               { clear(); copy(v); }
    void set(int8_t v)                             { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint8_t v)                            { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int16_t v)                            { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint16_t v)                           { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int32_t v)                            { clear(); copy(static_cast<int64_t>(v)); }
    void set(uint32_t v)                           { clear(); copy(static_cast<uint64_t>(v)); }
    void set(int64_t v)                            { clear(); copy(v); }
    void set(uint64_t v)                           { clear(); copy(v); }
    void set(double v)                             { clear(); copy(v); }
    void set(const std::string& v)                 { clear(); copy(v); }
    void set(const char *v)                        { clear(); copy(v); }
    void set(const std::vector<json>& v)           { clear(); copy(v); }
    void set(const std::map<std::string, json>& v) { clear(); copy(v); }

    void operator=(const json &v)                        { set(v); }
    void operator=(bool v)                               { set(v); }
    void operator=(int8_t v)                             { set(v); }
    void operator=(uint8_t v)                            { set(v); }
    void operator=(int16_t v)                            { set(v); }
    void operator=(uint16_t v)                           { set(v); }
    void operator=(int32_t v)                            { set(v); }
    void operator=(uint32_t v)                           { set(v); }
    void operator=(int64_t v)                            { set(v); }
    void operator=(uint64_t v)                           { set(v); }
    void operator=(double v)                             { set(v); }
    void operator=(const std::string &v)                 { set(v); }
    void operator=(const char *v)                        { set(v); }
    void operator=(const std::vector<json>& v)           { set(v); }
    void operator=(const std::map<std::string, json>& v) { set(v); }

    operator bool()        { return toBool();     }
    operator int8_t()      { return toSigned();   }
    operator uint8_t()     { return toUnsigned(); }
    operator int16_t()     { return toSigned();   }
    operator uint16_t()    { return toUnsigned(); }
    operator int32_t()     { return toSigned();   }
    operator uint32_t()    { return toUnsigned(); }
    operator int64_t()     { return toSigned();   }
    operator uint64_t()    { return toUnsigned(); }
    operator double()      { return toReal();     }
    operator std::string() { return toString();   }

    // JSON array:
    void add(const json &j);
    void addUndefined()                            { add(_undefined()); }
    void addNil()                                  { add(_null());      }
    void add(bool v)                               { add(json(v));      }
    void add(int64_t v)                            { add(json(v));      }
    void add(uint64_t v)                           { add(json(v));      }
    void add(double v)                             { add(json(v));      }
    void add(const std::string& v)                 { add(json(v));      }
    void add(const std::vector<json>& v)           { add(json(v));      }
    void add(const std::map<std::string, json>& v) { add(json(v));      }

    // JSON object:
    void add(const std::string &key, const json &j);
    void addUndefined(const std::string &key)                              { add(key, _undefined()); }
    void addNil(const std::string &key)                                    { add(key, _null());      }
    void add(const std::string &key, bool v)                               { add(key, json(v));      }
    void add(const std::string &key, int64_t v)                            { add(key, json(v));      }
    void add(const std::string &key, uint64_t v)                           { add(key, json(v));      }
    void add(const std::string &key, double v)                             { add(key, json(v));      }
    void add(const std::string &key, const std::string& v)                 { add(key, json(v));      }
    void add(const std::string &key, const std::vector<json>& v)           { add(key, json(v));      }
    void add(const std::string &key, const std::map<std::string, json>& v) { add(key, json(v));      }

    void clear();
    DataType getDataType() const { return type; }

    void write(std::ostream &os) const;
    std::string write() const { std::ostringstream os; write(os); return os.str(); }
    void parse(std::istream &is);
    void parse(const std::string &s) { std::istringstream is(s); parse(is); }

private:
    void copy(const json& v);
    void copy(bool v);
    void copy(int64_t v);
    void copy(uint64_t v);
    void copy(double v);
    void copy(const std::string& v);
    void copy(const char *v) { copy(std::string(v)); }
    void copy(const std::vector<json>& v);
    void copy(const std::map<std::string, json>& v);
    void copy(const std::vector<uint8_t>& v);
    void parse(scanner &sc);

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

    DataType type{UNDEFINED};
    union {
        bool                         bool_value;
        uint64_t                     uint_value;
        int64_t                      int_value;
        double                       real_value;
        std::string                 *string_value;
        std::vector<json>           *array_value;
        std::map<std::string, json> *object_value;
    };
}; // end class json //

const json undefined{json::_undefined()};
const json null{json::_null()};

} // end namespace jsonx //

#endif // JSONX_HPP
