#include "jsonx.hpp"
#include "io.hpp"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <assert.h>

using namespace std;

namespace jsonx {

const json           json::undefined(json::UNDEFINED_T);
const json           json::null(json::NULL_T);
const json_array_t   json::empty_array{};
const json_object_t  json::empty_object{};
const std::string    json::empty_string{""};

json jarray(initializer_list<json> args)
{
    json rg(json::empty_array);
    for_each(args.begin(), args.end(), [&rg](const json& j) {
        rg.add(j);
    });
    return rg;
}

json jobject(initializer_list<json_object_value_t> args)
{
    json obj(json::empty_object);
    for_each(args.begin(), args.end(), [&obj](const json_object_value_t& v) {
        obj.add(v.first, v.second);
    });
    return obj;
}

json::json(DataType t)
{
    type = t;
    switch (type) {
    case STRING_T:
        string_value = new string();
        break;
    case ARRAY_T:
        array_value = new json_array_t();
        break;
    case OBJECT_T:
        object_value = new json_object_t();
        break;
    default:
        break;
    } // end switch //
}

json::json(json&& rhs)
{
    if (this == &rhs)
        return;
    swap(type, rhs.type);
    switch (type) {
    case UNDEFINED_T:
        break;
    case NULL_T:
        break;
    case BOOL_T:
        bool_value = rhs.bool_value;
        break;
    case SIGNED_T:
        int_value = rhs.int_value;
        break;
    case UNSIGNED_T:
        uint_value = rhs.uint_value;
        break;
    case REAL_T:
        real_value = rhs.real_value;
        break;
    case STRING_T:
        swap(string_value, rhs.string_value);
        break;
    case ARRAY_T:
        swap(array_value, rhs.array_value);
        break;
    case OBJECT_T:
        swap(object_value, rhs.object_value);
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
}

json::~json()
{
    clear();
}

size_t json::size() const
{
    switch (type) {
    case UNDEFINED_T:
        return 0;
    case ARRAY_T:
        return array_value->size();
    case OBJECT_T:
    {
        size_t _size{0};
        for_each(object_value->begin(), object_value->end(),
                 [&_size](const json_object_value_t& p) {
           if (p.second.isDefined())
               _size += 1;
        });
        return _size;
    }
    default:
        return 1;
    } // end switch //
}

void json::clear()
{
    switch (type) {
    case UNDEFINED_T:
    case NULL_T:
    case BOOL_T:
    case SIGNED_T:
    case UNSIGNED_T:
    case REAL_T:
        break;
    case STRING_T:
        delete string_value;
        break;
    case ARRAY_T:
        delete array_value;
        break;
    case OBJECT_T:
        delete object_value;
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
    type = UNDEFINED_T;
}

void json::setNull()
{
    clear();
    type = NULL_T;
}

void json::copyFrom(const json& v)
{
    type = v.type;
    switch (type) {
    case UNDEFINED_T:
    case NULL_T:
        break;
    case BOOL_T:
        bool_value = v.toBool();
        break;
    case SIGNED_T:
        int_value = v.toSigned();
        break;
    case UNSIGNED_T:
        uint_value = v.toUnsigned();
        break;
    case REAL_T:
        real_value = v.toReal();
        break;
    case STRING_T:
        copyFrom(v.string_value->c_str());
        break;
    case ARRAY_T:
        copyFrom(v.toArray());
        break;
    case OBJECT_T:
        copyFrom(v.toObject());
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
}

void json::copyFrom(bool v)
{
    type = BOOL_T;
    bool_value = v;
}

void json::copyFrom(int64_t v)
{
    type = SIGNED_T;
    int_value = v;
}

void json::copyFrom(uint64_t v)
{
    type = UNSIGNED_T;
    uint_value = v;
}

void json::copyFrom(json_real_t v)
{
    type = REAL_T;
    real_value = v;
}

void json::copyFrom(const char *v)
{
    type = STRING_T;
    string_value = new string(v);
}

void json::copyFrom(const json_array_t& v)
{
    type = ARRAY_T;
    array_value = new json_array_t();
    for_each(v.begin(), v.end(), [this](const json& j) {
        array_value->push_back(json(j));
    });
}

void json::copyFrom(const json_object_t& v)
{
    type = OBJECT_T;
    object_value = new json_object_t();
    for_each(v.begin(), v.end(), [this](const json_object_value_t& p) {
        object_value->emplace(p.first, p.second);
    });
}

bool& json::toBoolRef()
{
    if (type != BOOL_T)
        set(toBool());
    return bool_value;
}

bool json::toBool() const
{
    switch (type) {
    case UNDEFINED_T:
        return false;
    case NULL_T:
        return false;
    case BOOL_T:
        return bool_value;
    case SIGNED_T:
        return int_value != 0;
    case UNSIGNED_T:
        return uint_value != 0;
    case REAL_T:
        return round(real_value) != 0;
    case STRING_T:
        return stoi(*string_value) != 0;
    case ARRAY_T:
        return !array_value->empty();
    case OBJECT_T:
        return !object_value->empty();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return false;
    } // end switch //
}

int64_t& json::toSignedRef64()
{
    if (type != SIGNED_T)
        set(toSigned());
    return int_value;
}

int32_t& json::toSignedRef32()
{
    return reinterpret_cast<int32_t&>(toSignedRef64());
}

int16_t& json::toSignedRef16()
{
    return reinterpret_cast<int16_t&>(toSignedRef64());
}

int8_t& json::toSignedRef8()
{
    return reinterpret_cast<int8_t&>(toSignedRef64());
}

int64_t json::toSigned() const
{
    switch (type) {
    case UNDEFINED_T:
        return 0;
    case NULL_T:
        return 0;
    case BOOL_T:
        return bool_value ? 1 : 0;
    case SIGNED_T:
        return int_value;
    case UNSIGNED_T:
        return uint_value;
    case REAL_T:
        return static_cast<int64_t>(round(real_value));
    case STRING_T:
        return stoi(*string_value);
    case ARRAY_T:
        return array_value->size();
    case OBJECT_T:
        return object_value->size();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

uint64_t& json::toUnsignedRef64()
{
    if (type != UNSIGNED_T)
        set(toUnsigned());
    return uint_value;
}

uint32_t& json::toUnsignedRef32()
{
    return reinterpret_cast<uint32_t&>(toSignedRef64());
}

uint16_t& json::toUnsignedRef16()
{
    return reinterpret_cast<uint16_t&>(toSignedRef64());
}

uint8_t& json::toUnsignedRef8()
{
    return reinterpret_cast<uint8_t&>(toSignedRef64());
}

uint64_t json::toUnsigned() const
{
    switch (type) {
    case UNDEFINED_T:
        return 0;
    case NULL_T:
        return 0;
    case BOOL_T:
        return bool_value ? 1 : 0;
    case SIGNED_T:
        if (int_value >= 0)
            return int_value;
        else
            return 0;
    case UNSIGNED_T:
        return uint_value;
    case REAL_T:
        if (real_value > -0.5)
            return static_cast<uint64_t>(round(real_value));
        else
            return 0;
    case STRING_T:
        return stoi(*string_value);
    case ARRAY_T:
        return array_value->size();
    case OBJECT_T:
        return object_value->size();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

json_real_t& json::toRealRef()
{
    if (type != REAL_T)
        set(toReal());
    return real_value;
}

json_real_t json::toReal() const
{
    switch (type) {
    case UNDEFINED_T:
        return 0.0;
    case NULL_T:
        return 0.0;
    case BOOL_T:
        return static_cast<json_real_t>(bool_value ? 1.0 : 0.0);
    case SIGNED_T:
        return static_cast<json_real_t>(int_value);
    case UNSIGNED_T:
        return static_cast<json_real_t>(uint_value);
    case REAL_T:
        return real_value;
    case STRING_T:
        return static_cast<json_real_t>(stoi(*string_value));
    case ARRAY_T:
        return static_cast<json_real_t>(array_value->size());
    case OBJECT_T:
        return static_cast<json_real_t>(object_value->size());
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

std::string json::toString() const
{
    if (type == STRING_T)
        return *string_value;
    else
        return empty_string;
}

std::string json::toString()
{
    switch (type) {
    case UNDEFINED_T:
        return "";
    case NULL_T:
        return "null";
    case BOOL_T:
        return bool_value ? "true" : "false";
    case SIGNED_T:
        return to_string(int_value);
    case UNSIGNED_T:
        return to_string(uint_value);
    case REAL_T:
        return to_string(real_value);
    case STRING_T:
        return *string_value;
    case ARRAY_T:
    case OBJECT_T:
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

std::string& json::toStringRef()
{
    if (type != STRING_T) {
        set(toString());
    }
    return *string_value;
}

const std::string& json::toStringRef() const
{
    if (type != STRING_T) {
        return empty_string;
    } else {
        return *string_value;
    }
}

const json_array_t& json::toArray() const
{
    if (type == ARRAY_T)
        return *array_value;
    else
        return empty_array;
}

json_array_t json::toArray()
{
    json_array_t j;
    switch (type) {
    case UNDEFINED_T:
        break;
    case NULL_T:
        break;
    case BOOL_T:
        j.push_back(json(bool_value ? "true" : "false"));
        break;
    case SIGNED_T:
        j.push_back(json(int_value));
        break;
    case UNSIGNED_T:
        j.push_back(json(uint_value));
        break;
    case REAL_T:
        j.push_back(json(real_value));
        break;
    case STRING_T:
        j.push_back(json(string_value->c_str()));
        break;
    case ARRAY_T:
        for_each(array_value->begin(), array_value->end(),
                 [&j](const json& v) {
            j.push_back(v);
        });
        break;
    case OBJECT_T:
        for_each(object_value->begin(), object_value->end(),
                 [&j](const json_object_value_t& v) {
            j.push_back(v.second);
        });
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
    return j;
}

json_array_t& json::toArrayRef()
{
    if (type != ARRAY_T) {
        set(toArray());
    }
    return *array_value;
}

const json_array_t& json::toArrayRef() const
{
    if (type != ARRAY_T) {
        throw runtime_error("Not an array");
    }
    return *array_value;
}

const json_object_t& json::toObject() const
{
    if (type == OBJECT_T)
        return *object_value;
    else
        return empty_object;
}

json_object_t json::toObject()
{
    json_object_t m;
    size_t i{0};
    switch (type) {
    case UNDEFINED_T:
        break;
    case NULL_T:
        break;
    case BOOL_T:
        m.emplace("0", json(bool_value ? "true" : "false"));
        break;
    case SIGNED_T:
        m.emplace("0", json(int_value));
        break;
    case UNSIGNED_T:
        m.emplace("0", json(uint_value));
        break;
    case REAL_T:
        m.emplace("0", json(real_value));
        break;
    case STRING_T:
        m.emplace("0", json(string_value->c_str()));
        break;
    case ARRAY_T:
        for_each(array_value->begin(), array_value->end(),
                 [&i, &m](const json& v) {
            m.emplace(to_string(i++), v);
        });
        break;
    case OBJECT_T:
        for_each(object_value->begin(), object_value->end(),
                 [&m](const json_object_value_t& v) {
            m.emplace(v.first, v.second);
        });
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //}
    return m;
}

json_object_t& json::toObjectRef()
{
    if (type == OBJECT_T) {
        set(toObject());
    }
    return *object_value;
}

const json_object_t& json::toObjectRef() const
{
    if (type == OBJECT_T) {
        throw runtime_error("Not an object");
    }
    return *object_value;
}

json& json::at(size_t i)
{
    if (type != ARRAY_T) {
        set(toArray());
        for (size_t j = 0; j < i; ++j)
            (*array_value)[j].setUndefined();
    } else {
        while(i >= array_value->size())
            array_value->push_back(undefined);
    }
    return (*array_value)[i];
}

const json& json::at(size_t i) const
{
    if ((type != ARRAY_T) || (i >= size()))
        return undefined;
    return (*array_value)[i];
}

json& json::find(const char *key)
{
    json_object_t::iterator iter;
    if (type != OBJECT_T) {
        set(toObject());
        iter = object_value->end();
    } else {
        iter = object_value->find(key);
    }
    if (iter == object_value->end())
        iter = object_value->emplace(key, undefined).first;
    return iter->second;
}

const json& json::find(const char *key) const
{
    if (type != OBJECT_T)
        return undefined;
    json_object_t::const_iterator iter = object_value->find(key);
    if (iter == object_value->end())
        return undefined;
    return iter->second;
}

void json::add(const json &j)
{
    if (type != ARRAY_T) {
        clear();
        array_value = new json_array_t();
        type = ARRAY_T;
    }
    if (j.isDefined())
        array_value->push_back(j);
}

void json::add(const string &key, const json &j)
{
    if (type != OBJECT_T) {
        clear();
        object_value = new json_object_t();
        type = OBJECT_T;
    }
    auto iter = object_value->find(key);
    if (iter != object_value->end())
        object_value->erase(iter);
    object_value->emplace(key, j);
}

bool json::operator==(const json &v) const
{
    if (type != v.type)
        return false;
    switch (type) {
    case UNDEFINED_T:
        return false; // Undefined never equals anything!
    case NULL_T:
        return true;
    case BOOL_T:
        return (bool_value == v.bool_value);
    case SIGNED_T:
        return (int_value == v.int_value);
    case UNSIGNED_T:
        return (uint_value == v.uint_value);
    case REAL_T:
        return (real_value == v.real_value);
    case STRING_T:
        return (*string_value == *v.string_value);
    case ARRAY_T:
        return operator==(*v.array_value);
    case OBJECT_T:
        return operator==(*v.object_value);
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return false;
    } // end switch //
}

bool json::operator==(const json_array_t &v) const
{
    if ((type != ARRAY_T) || (size() != v.size()))
        return false;
    for (size_t i = 0; i < size(); ++i) {
        if (!((*array_value)[i]).operator==(v[i]))
            return false;
    } // end for //
    return true;
}

bool json::operator==(const json_object_t &v) const
{
    if ((type != OBJECT_T) || (size() != v.size()))
        return false;
    auto result_iter = find_if_not(object_value->begin(), object_value->end(),
                                   [&v](const json_object_value_t &left) {
        const string& left_key = left.first;
        const json& left_val = left.second;
        if (left_val.isDefined()) {
            auto iter = v.find(left_key);
            return ((iter != v.end() && left_val.operator==(iter->second)));
        } else {
            return true;
        }
    });
    return (result_iter == object_value->end());
}

} // end namespace jsonx //
