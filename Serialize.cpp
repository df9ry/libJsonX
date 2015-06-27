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

#include <B64Coder.h>

#include <sstream>

using namespace std;

namespace JsonX {

	std::string Value::toJsonString() const {
		stringstream os{};
		toJson(os);
		return os.str();
	}

	void Value::toJson(std::ostream& os) const {
		switch (m_type) {
		case Type::NONE:
			os << "null";
			break;
		case Type::SIGNED:
			os << v_signed;
			break;
		case Type::UNSIGNED:
			os << v_unsigned;
			break;
		case Type::BOOL:
			os << (v_bool ? "true" : "false");
			break;
		case Type::FLOAT:
			os << v_float;
			break;
		case Type::BLOB:
			asConstBlob().toJson(os);
			break;
		case Type::STRING:
			asConstString().toJson(os);
			break;
		case Type::LIST:
			asConstList().toJson(os);
			break;
		case Type::OBJECT:
			asConstObject().toJson(os);
			break;
		default:
			throw runtime_error("Invalid JsonX::Type: " + static_cast<int>(m_type));
		} // end switch //
	}

	void Blob::toJson(std::ostream& os) const {
		const BlobValue& v{ get() };
		vector<uint8_t>::const_iterator i = v.begin();
		os << '=';
		B64::encode([&i, &v]() {
			int c = (i != v.end()) ? *i : -1;
			i++;
			return c;
		}, [&os](char ch){
			os << ch;
		});
		os << '=';
	}

	void String::toJson(std::ostream& os, const std::string& s) {
		os << '"';
		for (string::const_iterator i = s.begin();
			i != s.end(); ++i)
		{
			char ch = *i;
			switch (ch)
			{
			case '"':
				os << "\\\"";
				break;
			case '\\':
				os << "\\\\";
				break;
			case '/':
				os << "\\/";
				break;
			case '\b':
				os << "\\b";
				break;
			case '\f':
				os << "\\f";
				break;
			case '\n':
				os << "\\n";
				break;
			case '\r':
				os << "\\r";
				break;
			case '\t':
				os << "\\t";
				break;
			default:
				if (iscntrl(ch))
				{
					char b[7];
# ifdef _WIN32
					_snprintf_s(b, sizeof(b), _TRUNCATE, "\\u%04x", static_cast<int>(ch));
#else
					snprintf(b, sizeof(b), "\\u%04x", static_cast<int>(ch));
#endif
					os << b;
				}
				else
				{
					os << ch;
				}
				break;
			} // end switch //
		} // end for //
		os << '"';
	}

	void String::toJson(std::ostream& os) const {
		String::toJson(os, get());
	}

	void List::toJson(std::ostream& os) const {
		const ListValue& l{ get() };
		bool first{ true };
		os << '[';
		for (auto i = l.begin(); i != l.end(); ++i) {
			if (first) first = false; else os << ',';
			const Value* v{ i->get() };
			if (v != nullptr) v->toJson(os); else os << "null";
		} // end for //
		os << ']';
	}

	void Object::toJson(std::ostream& os) const {
		const ObjectValue& o{ get() };
		bool first{ true };
		os << '{';
		for (auto i = o.begin(); i != o.end(); ++i) {
			if (first) first = false; else os << ',';
			String::toJson(os, i->first);
			os << ":";
			const Value* v{ i->second.get() };
			if (v != nullptr) v->toJson(os); else os << "null";
		} // end for //
		os << '}';
	}

} // end namespace JsonX //
