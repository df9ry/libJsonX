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

#include "JsonXObject.h"
#include "JsonXException.h"
#include "JsonXString.h"
#include "JsonXNull.h"

#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace JsonX {

JsonXObject::JsonXObject(): m_value
	{unique_ptr<JsonXObjectValue>(new JsonXObjectValue())} {}

JsonXObject::JsonXObject(initializer_list<JsonXObjectInitEntry> il): m_value
	{unique_ptr<JsonXObjectValue>(new JsonXObjectValue())}
{
	for (initializer_list<JsonXObjectInitEntry>::const_iterator i = il.begin();
			i != il.end(); ++ i)
		add(i->key, i->val);
}

JsonXObject::~JsonXObject() {}

JsonXObject* JsonXObject::add(const string& key, JsonXBase* val) {
	m_value.get()->push_back(unique_ptr<JsonXObjectEntry>(
			new JsonXObjectEntry{key, unique_ptr<JsonXBase>(val)}));
	return this;
}

string JsonXObject::toString() const {
	ostringstream oss{};
	oss << '{';
	bool first{true};
	oss << '[';
	for (JsonXObjectValue::const_iterator i = m_value.get()->begin();
			i != m_value.get()->end(); ++i) {
		if (first) first = false; else oss << ',';
		const JsonXObjectEntry* e{i->get()};
		if (e) {
			oss << JsonXString::toJson(e->key);
			oss << ':';
			JsonXBase* v{e->val.get()};
			oss << (v) ? v->toString() : "null";
		} else {
			oss << "null";
		}
	} // end for //
	oss << '}';
	return oss.str();
}

JsonXObject* JsonXObject::read(istream& iss) {
    int next_ch;
    next_ch = (char)skipWhitespace(iss);
    if (next_ch == -1)
        throw JsonXException(
        		"JsonXObject::read(): Premature end of file");
    if (next_ch != '{')
        throw JsonXException(
            "Character '{' expected. Got: '" +
			to_string(static_cast<char>(next_ch)) + "'");
    JsonXObject* items = new JsonXObject();
    readChar(iss);;
    next_ch = skipWhitespace(iss);
    if (next_ch != '}') {
        while (true) {
            string&& key = move(JsonXString::readRaw(iss));
            next_ch = skipWhitespace(iss);
            if (next_ch != ':')
                throw JsonXException(
                    "Expected ':'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
            readChar(iss);;
            JsonXBase* val = JsonXBase::read(iss);
            items->add(key, move(val));
            next_ch = skipWhitespace(iss);
            if (next_ch == '}')
                break;
            if (next_ch != ',')
                throw JsonXException(
                    "Expected ',' or '}'. Got: '" +
					to_string(static_cast<char>(next_ch)) + "'");
            readChar(iss);;
        } // end while //
    }
    readChar(iss);
    return items;
}

bool JsonXObject::contains(const string& key) {
	const JsonXObjectValue& l = *m_value.get();
	for (JsonXObjectValue::const_iterator i = l.begin();
			i != l.end(); ++i)
		if (i->get() && (i->get()->key == key)) return true;
	return false;
}

static JsonXNull jxnull{};

const JsonXBase& JsonXObject::find(const string& key) const {
	const JsonXObjectValue& l = *m_value.get();
	for (JsonXObjectValue::const_iterator i = l.begin();
			i != l.end(); ++i)
		if (i->get() && (i->get()->key == key))
			return *(i->get()->val.get());
	return jxnull;
}

JsonXBase&& JsonXObject::find(const string& key) {
	const JsonXObjectValue& l = *m_value.get();
	for (JsonXObjectValue::const_iterator i = l.begin();
			i != l.end(); ++i)
		if (i->get() &&(i->get()->key == key))
			return move(*(i->get()->val.get()));
	throw invalid_argument("Not found: " + key);
}

unique_ptr<JsonXObjectValue> JsonXObject::extract() {
	unique_ptr<JsonXObjectValue> d{unique_ptr<JsonXObjectValue>()};
	swap(m_value, d);
	return d;
}

} /* namespace JsonX */
