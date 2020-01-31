
#include "jsonx.hpp"
#include "io.hpp"
#include "scanner.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <regex>
#include <cstdint>

using namespace std;

static const regex UINT_EXPR(R"(^\d+$)");
static const regex SINT_EXPR(R"(^[+-]\d+$)");
static const regex REAL_EXPR(R"(^-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?$)");

namespace jsonx {

void serialize(ostream &ss, const string &v) {
    ss << "\"";
    for_each(v.begin(), v.end(), [&ss](const char ch) {
        if (ch == '\\' || ch == '"')
            ss << "\\";
        ss << ch;
    });
    ss << "\"";
}

void serialize(ostream &ss, const vector<json> &v) {
    ss << "[";
    bool first{true};
    for_each (v.begin(), v.end(), [&first, &ss] (const json& v) {
        if (first)
            first = false;
        else
            ss << ",";
        v.write(ss);
    });
    ss << "]";
}

void serialize(ostream &ss, const map<string, json> &v) {
    ss << "{";
    bool first{true};
    for_each (v.begin(), v.end(), [&first, &ss] (const pair<string, json> &v) {
        if (first)
            first = false;
        else
            ss << ",";
        serialize(ss, v.first);
        ss << ":";
        v.second.write(ss);
    });
    ss << "}";
}

void json::write(std::ostream &os) const
{
    switch (type) {
    case UNDEFINED:
        os << "undefined";
        break;
    case NIL:
        os << "null";
        break;
    case BOOLEAN:
        os << (bool_value ? "true" : "false");
        break;
    case SIGNED_INT:
        os << int_value;
        break;
    case UNSIGNED_INT:
        os << uint_value;
        break;
    case DOUBLE:
        os << real_value;
        break;
    case STRING:
        ::jsonx::serialize(os, *string_value);
        break;
    case ARRAY:
        ::jsonx::serialize(os, *array_value);
        break;
    case OBJECT:
        ::jsonx::serialize(os, *object_value);
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //}
}

static void parse_string(jsonx::scanner &sc, std::string &s)
{
    sc.skip_whitespace();
    if (sc.cur_ch != '"') {
        char s[2];
        s[0] = sc.cur_ch;
        s[1] = '\0';
        throw runtime_error(string("Expected ':', got '") + s + "'");
    }
    sc.get_ch();
    bool esc{false};
    std::ostringstream is;
    while ((sc.cur_ch != '"') && !esc) {
        if (sc.eof())
            throw runtime_error("Premature EOF");
        if ((sc.cur_ch == '\\') && !esc) {
            esc = true;
        } else {
            is.put(sc.cur_ch);
            esc = false;
        }
        sc.get_ch();
    } // end while //
    sc.get_ch();
    s = is.str();
}

static void parse_token(jsonx::scanner &sc, std::string &s)
{
    sc.skip_whitespace();
    std::ostringstream is;
    while ((!sc.eof())
           && (!std::isspace(sc.cur_ch))
           && (sc.cur_ch != ',')
           && (sc.cur_ch != ']')
           && (sc.cur_ch != '}'))
    {
        is.put(sc.cur_ch);
        sc.get_ch();
    }
    s = is.str();
}

void json::parse(jsonx::scanner &sc)
{
    clear();
    sc.skip_whitespace();
    switch (sc.cur_ch) {
    case '{':
        {
            type = OBJECT;
            object_value = new std::map<std::string, json>();
            sc.get_ch();
            sc.skip_whitespace();
            while (sc.cur_ch != '}') {
                if (sc.eof())
                    throw runtime_error("Premature EOF");
                std::string key;
                jsonx::parse_string(sc, key);
                sc.skip_whitespace();
                if (sc.cur_ch != ':') {
                    char s[2];
                    s[0] = sc.cur_ch;
                    s[1] = '\0';
                    throw runtime_error(string("Expected ':', got '") + s + "'");
                }
                sc.get_ch();
                sc.skip_whitespace();
                json child;
                child.parse(sc);
                add(key, child);
                sc.skip_whitespace();
                if (sc.cur_ch == ',') {
                    sc.get_ch();
                    sc.skip_whitespace();
                }
            } // end while //
            sc.get_ch();
        }
        break;
    case '[':
        {
            type = ARRAY;
            array_value = new std::vector<json>();
            sc.get_ch();
            sc.skip_whitespace();
            while (sc.cur_ch != ']') {
                if (sc.eof())
                    throw runtime_error("Premature EOF");
                json child;
                child.parse(sc);
                add(child);
                sc.skip_whitespace();
                if (sc.cur_ch == ',') {
                    sc.get_ch();
                    sc.skip_whitespace();
                }
            } // end while //
        }
        break;
    case '"':
        {
            std::string s;
            jsonx::parse_string(sc, s);
            set(s);
        }
        break;
    default:
        {
            std::string s;
            jsonx::parse_token(sc, s);
            if (s == "null") {
                type = NIL;
                return;
            }
            if (s == "undefined") {
                type = UNDEFINED;
                return;
            }
            if (s == "true") {
                type = BOOLEAN;
                bool_value = true;
                return;
            }
            if (s == "false") {
                type = BOOLEAN;
                bool_value = false;
                return;
            }
            if (regex_match(s, UINT_EXPR)) {
                type = UNSIGNED_INT;
                uint_value = std::stoull(s);
                return;
            }
            if (regex_match(s, SINT_EXPR)) {
                type = SIGNED_INT;
                uint_value = std::stoll(s);
                return;
            }
            if (regex_match(s, REAL_EXPR)) {
                type = DOUBLE;
                real_value = std::stod(s);
                return;
            }
            throw runtime_error(string("Unexpected token: \"" + s + "\""));
        }
    } // end switch //
}

void json::parse(std::istream &is)
{
    scanner sc(is);
    try {
        parse(sc);
    }
    catch (const std::exception &ex) {
        clear();
        throw runtime_error(std::string("Syntax error ") + ex.what()
                            + " in line " + std::to_string(sc.cur_line)
                            + ", column " + std::to_string(sc.cur_col));
    }
    catch (...) {
        clear();
        throw runtime_error(std::string("Syntax error ...")
                            + " in line " + std::to_string(sc.cur_line)
                            + ", column " + std::to_string(sc.cur_col));
    }
}

} // end namespace jsonx //
