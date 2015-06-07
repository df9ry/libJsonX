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

#include "JsonXBlob.h"
#include "JsonXException.h"
#include "B64Coder.h"

using namespace std;
using namespace B64;

namespace JsonX {

JsonXBlob::JsonXBlob(): m_value
	{unique_ptr<JsonXBlobData>(new JsonXBlobData())} {}

JsonXBlob::JsonXBlob(const JsonXBlobData& value): m_value
	{unique_ptr<JsonXBlobData>(new JsonXBlobData(value))} {}

JsonXBlob::JsonXBlob(const uint8_t* pb, size_t cb):
		m_value{new JsonXBlobData(cb)}
{
	uninitialized_copy(pb, pb + cb, m_value.get()->begin());
}

JsonXBlob::JsonXBlob(JsonXBlobData&& value): m_value
	{unique_ptr<JsonXBlobData>(move(&value))} {}

JsonXBlob::JsonXBlob(initializer_list<uint8_t> il):
		m_value{new JsonXBlobData(il.size())}
{
	uninitialized_copy(il.begin(), il.begin() + il.size(),
			m_value.get()->begin());
}

JsonXBlob::~JsonXBlob() {}

string JsonXBlob::toString() const {
	ostringstream oss{};
	auto i = m_value.get()->begin();
	encode( [&i, this](){ return (i != m_value.get()->end()) ? *++i : -1; },
			[&oss](char ch){ oss << ch; });
	return oss.str();
}

JsonXBlob* JsonXBlob::read(istream& iss) {
    int next_ch = skipWhitespace(iss);
    if (next_ch != '=')
        throw JsonXException(
            "Invalid input: Expected '=', got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
    readChar(iss);
    auto pValue = new JsonXBlob();
	decode( [&iss](){ char ch; return iss.readsome(&ch, 1) ? ch : -1; },
			[&pValue](uint8_t x){ pValue->add(x); });
	// Read away trailing '=':
	while (iss.peek() == '=')
		readChar(iss);
    return pValue;
}

unique_ptr<JsonXBlobData> JsonXBlob::extract() {
	unique_ptr<JsonXBlobData> d{unique_ptr<JsonXBlobData>()};
	swap(d, m_value);
	return d;
}

} /* namespace JsonX */