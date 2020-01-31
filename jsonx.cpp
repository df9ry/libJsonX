#include "jsonx.hpp"
#include "io.hpp"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <assert.h>

using namespace std;
using namespace jsonx;

json::json(json&& rhs)
{
    if (this == &rhs)
        return;
    swap(type, rhs.type);
    switch (type) {
    case UNDEFINED:
        break;
    case NIL:
        break;
    case BOOLEAN:
        bool_value = rhs.bool_value;
        break;
    case SIGNED_INT:
        int_value = rhs.int_value;
        break;
    case UNSIGNED_INT:
        uint_value = rhs.uint_value;
        break;
    case DOUBLE:
        real_value = rhs.real_value;
        break;
    case STRING:
        swap(string_value, rhs.string_value);
        break;
    case ARRAY:
        swap(array_value, rhs.array_value);
        break;
    case OBJECT:
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

void json::clear()
{
    switch (type) {
    case UNDEFINED:
    case NIL:
    case BOOLEAN:
    case SIGNED_INT:
    case UNSIGNED_INT:
    case DOUBLE:
        break;
    case STRING:
        delete string_value;
        break;
    case ARRAY:
        delete array_value;
        break;
    case OBJECT:
        delete object_value;
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
    type = UNDEFINED;
}

void json::setNil()
{
    clear();
    type = NIL;
}

void json::copy(const json& v)
{
    type = v.type;
    switch (type) {
    case UNDEFINED:
    case NIL:
        break;
    case BOOLEAN:
        bool_value = v.toBool();
        break;
    case SIGNED_INT:
        int_value = v.toSigned();
        break;
    case UNSIGNED_INT:
        uint_value = v.toUnsigned();
        break;
    case DOUBLE:
        real_value = v.toReal();
        break;
    case STRING:
        copy(v.toString());
        break;
    case ARRAY:
        copy(v.toArray());
        break;
    case OBJECT:
        copy(v.toObject());
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
}

void json::copy(bool v)
{
    type = BOOLEAN;
    bool_value = v;
}

void json::copy(int64_t v)
{
    type = SIGNED_INT;
    int_value = v;
}

void json::copy(uint64_t v)
{
    type = UNSIGNED_INT;
    uint_value = v;
}

void json::copy(double v)
{
    type = DOUBLE;
    real_value = v;
}

void json::copy(const string& v)
{
    type = STRING;
    string_value = new string(v);
}

void json::copy(const vector<json>& v)
{
    type = ARRAY;
    array_value = new vector<json>();
    for_each(v.begin(), v.end(), [this](const json& j) {
        array_value->push_back(json(j));
    });
}

void json::copy(const std::map<string, json>& v)
{
    type = OBJECT;
    object_value = new map<string, json>();
    for_each(v.begin(), v.end(), [this](const pair<string, json> p) {
        object_value->emplace(p.first, p.second);
    });
}

bool json::toBool() const
{
    switch (type) {
    case UNDEFINED:
        return false;
    case NIL:
        return false;
    case BOOLEAN:
        return bool_value;
    case SIGNED_INT:
        return int_value != 0;
    case UNSIGNED_INT:
        return uint_value != 0;
    case DOUBLE:
        return round(real_value) != 0;
    case STRING:
        return stoi(*string_value) != 0;
    case ARRAY:
        return !array_value->empty();
    case OBJECT:
        return !object_value->empty();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return false;
    } // end switch //
}

int64_t json::toSigned() const
{
    switch (type) {
    case UNDEFINED:
        return 0;
    case NIL:
        return 0;
    case BOOLEAN:
        return bool_value ? 1 : 0;
    case SIGNED_INT:
        return int_value;
    case UNSIGNED_INT:
        return uint_value;
    case DOUBLE:
        return round(real_value);
    case STRING:
        return stoi(*string_value);
    case ARRAY:
        return array_value->size();
    case OBJECT:
        return object_value->size();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

uint64_t json::toUnsigned() const
{
    switch (type) {
    case UNDEFINED:
        return 0;
    case NIL:
        return 0;
    case BOOLEAN:
        return bool_value ? 1 : 0;
    case SIGNED_INT:
        if (int_value >= 0)
            return int_value;
        else
            return 0;
    case UNSIGNED_INT:
        return uint_value;
    case DOUBLE:
        if (real_value > -0.5)
            return round(real_value);
        else
            return 0.0;
    case STRING:
        return stoi(*string_value);
    case ARRAY:
        return array_value->size();
    case OBJECT:
        return object_value->size();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

double json::toReal() const
{
    switch (type) {
    case UNDEFINED:
        return 0.0;
    case NIL:
        return 0.0;
    case BOOLEAN:
        return bool_value ? 1.0 : 0.0;
    case SIGNED_INT:
        return int_value;
    case UNSIGNED_INT:
        return uint_value;
    case DOUBLE:
        return real_value;
    case STRING:
        return stoi(*string_value);
    case ARRAY:
        return array_value->size();
    case OBJECT:
        return object_value->size();
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

const std::string json::toString() const
{
    switch (type) {
    case UNDEFINED:
        return "";
    case NIL:
        return "null";
    case BOOLEAN:
        return bool_value ? "true" : "false";
    case SIGNED_INT:
        return to_string(int_value);
    case UNSIGNED_INT:
        return to_string(uint_value);
    case DOUBLE:
        return to_string(real_value);
    case STRING:
        return *string_value;
    case ARRAY:
    case OBJECT:
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
        return 0;
    } // end switch //
}

const std::vector<json> json::toArray() const
{
    vector<json> j;
    switch (type) {
    case UNDEFINED:
        break;
    case NIL:
        break;
    case BOOLEAN:
        j.push_back(json(bool_value ? "true" : "false"));
        break;
    case SIGNED_INT:
        j.push_back(json(int_value));
        break;
    case UNSIGNED_INT:
        j.push_back(json(uint_value));
        break;
    case DOUBLE:
        j.push_back(json(real_value));
        break;
    case STRING:
        j.push_back(json(*string_value));
        break;
    case ARRAY:
        for_each(array_value->begin(), array_value->end(), [&j](const json& v) {
            j.push_back(v);
        });
        break;
    case OBJECT:
        for_each(object_value->begin(), object_value->end(), [&j](const pair<string, json>& v) {
            j.push_back(v.second);
        });
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //
    return j;
}

const std::map<string, json> json::toObject() const
{
    map<string, json> m;
    size_t i{0};
    switch (type) {
    case UNDEFINED:
        break;
    case NIL:
        break;
    case BOOLEAN:
        m.emplace("0", json(bool_value ? "true" : "false"));
        break;
    case SIGNED_INT:
        m.emplace("0", json(int_value));
        break;
    case UNSIGNED_INT:
        m.emplace("0", json(uint_value));
        break;
    case DOUBLE:
        m.emplace("0", json(real_value));
        break;
    case STRING:
        m.emplace("0", json(*string_value));
        break;
    case ARRAY:
        for_each(array_value->begin(), array_value->end(), [&i, &m](const json& v) {
            m.emplace(to_string(i++), v);
        });
        break;
    case OBJECT:
        for_each(object_value->begin(), object_value->end(), [&m](const pair<string, json>& v) {
            m.emplace(v.first, v.second);
        });
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //}
    return m;
}

void json::add(const json &j)
{
    if (type != ARRAY) {
        clear();
        array_value = new vector<json>();
        type = ARRAY;
    }
    array_value->push_back(j);
}

void json::add(const string &key, const json &j)
{
    if (type != OBJECT) {
        clear();
        object_value = new map<string, json>();
        type = OBJECT;
    }
    auto iter = object_value->find(key);
    if (iter != object_value->end())
        object_value->erase(iter);
    object_value->emplace(key, j);
}
