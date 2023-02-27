
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
        switch (ch) {
        case '\0':
            ss << "\\0";
            break;
        case '\"':
            ss << "\\\"";
            break;
        case '\\':
            ss << "\\\\";
            break;
        case '\b':
            ss << "\\b";
            break;
        case '\f':
            ss << "\\f";
            break;
        case '\n':
            ss << "\\n";
            break;
        case '\r':
            ss << "\\r";
            break;
        case '\t':
            ss << "\\t";
            break;
        default:
            ss << ch;
            break;
        } // end switch //
    });
    ss << "\"";
}

void serialize(ostream &ss, const json_array_t &v) {
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

void serialize(ostream &ss, const json_object_t &v) {
    ss << "{";
    bool first{true};
    for_each (v.begin(), v.end(), [&first, &ss] (const pair<const string, json> &v) {
        if (v.second.isDefined()) {
            if (first)
                first = false;
            else
                ss << ",";
            serialize(ss, v.first);
            ss << ":";
            v.second.write(ss);
        }
    });
    ss << "}";
}

void json::write(std::ostream &os) const
{
    switch (type) {
    case UNDEFINED_T:
        break;
    case NULL_T:
        os << "null";
        break;
    case BOOL_T:
        os << (bool_value ? "true" : "false");
        break;
    case SIGNED_T:
        os << int_value;
        break;
    case UNSIGNED_T:
        os << uint_value;
        break;
    case REAL_T:
        os << real_value;
        break;
    case STRING_T:
        ::jsonx::serialize(os, *string_value);
        break;
    case ARRAY_T:
        ::jsonx::serialize(os, *array_value);
        break;
    case OBJECT_T:
        ::jsonx::serialize(os, *object_value);
        break;
    default:
        cerr << "jsonx::json: Invalid data type " << type << endl;
    } // end switch //}
}

static inline std::string chartostring(char c)
{
    return string(&c, 1);
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
        if (!esc) {
            if (sc.cur_ch != '\\') {
                is.put(sc.cur_ch);
            } else {
                esc = true;
            }
        } else { // We have esc
            switch (sc.cur_ch) {
            case '0':
                is.put('\0');
                break;
            case '"':
                is.put('"');
                break;
            case '\\':
                is.put('\\');
                break;
            case 'b':
                is.put('\b');
                break;
            case 'f':
                is.put('\f');
                break;
            case 'n':
                is.put('\n');
                break;
            case 'r':
                is.put('\r');
                break;
            case 't':
                is.put('\t');
                break;
            case 'u': // Unicode not implemented for now
                throw runtime_error("Unicode escape sequences not implemented for now");
            default:
                throw runtime_error("Invalid escape sequence '\\"
                                    + chartostring(sc.cur_ch) + "'");
            } // end switch //
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
            type = OBJECT_T;
            object_value = new json_object_t();
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
            type = ARRAY_T;
            array_value = new json_array_t();
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
            sc.get_ch();
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
            if (s == "") {
                type = UNDEFINED_T;
                return;
            }
            if (s == "null") {
                type = NULL_T;
                return;
            }
            if (s == "true") {
                type = BOOL_T;
                bool_value = true;
                return;
            }
            if (s == "false") {
                type = BOOL_T;
                bool_value = false;
                return;
            }
            if (regex_match(s, UINT_EXPR)) {
                type = UNSIGNED_T;
                uint_value = std::stoull(s);
                return;
            }
            if (regex_match(s, SINT_EXPR)) {
                type = SIGNED_T;
                uint_value = std::stoll(s);
                return;
            }
            if (regex_match(s, REAL_EXPR)) {
                type = REAL_T;
                real_value = static_cast<json_real_t>(std::stod(s));
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
