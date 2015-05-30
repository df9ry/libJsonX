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
#include "JsonXBase.h"
#include "JsonXNull.h"
#include "JsonXString.h"
#include "JsonXNumber.h"

#include <cassert>
#include <stdexcept>

using namespace std;

namespace JsonX {

JsonXValue::JsonXValue():
	m_value{unique_ptr<JsonXBase>(new JsonXNull())}
{
}

JsonXValue::JsonXValue(JsonXValue&& other) {
	swap(m_value, other.m_value);
}

JsonXValue::JsonXValue(JsonXBase* v):
		m_value{unique_ptr<JsonXBase>(v)}
{
}

JsonXValue::~JsonXValue() {
}

JsonXValue& JsonXValue::operator=(JsonXValue&& other) {
	swap(m_value, other.m_value);
	return *this;
}

std::string JsonXValue::toString() const {
	assert(m_value);
	return m_value.get()->toString();
}

ValueType JsonXValue::type() const {
	assert(m_value);
	return m_value.get()->type();
}

std::string JsonXValue::asString() const {
	assert(m_value);
	const JsonXBase* v{m_value.get()};
	if (v->type() == ValueType::T_STRING) {
		const JsonXString* s = dynamic_cast<const JsonXString*>(v);
		assert(s);
		return s->value();
	}
	return v->toString();
}

int16_t JsonXValue::asInt16() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->int16Value();
}

uint16_t JsonXValue::asUInt16() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->uint16Value();
}

int32_t JsonXValue::asInt32() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->int32Value();
}

uint32_t JsonXValue::asUInt32() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->uint32Value();
}

int64_t JsonXValue::asInt64() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->int64Value();
}

uint64_t JsonXValue::asUInt64() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->uint64Value();
}

double JsonXValue::asDouble() const {
	assert(m_value);
	const JsonXNumber* n{dynamic_cast<const JsonXNumber*>(m_value.get())};
	if (!n) throw invalid_argument("Not numeric");
	return n->doubleValue();
}

void JsonXValue::write(std::ostream& oss) const {
	assert(m_value);
	oss << m_value.get()->toString();
}

unique_ptr<JsonXValue> JsonXValue::read(istream& iss) {
	return unique_ptr<JsonXValue>(new JsonXValue(JsonXBase::read(iss)));
}

void JsonXValue::setBlob(std::initializer_list<uint8_t> bytes) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	JsonXBlobData data{JsonXBlobData(bytes.size())};
	uninitialized_copy(bytes.begin(), bytes.begin() + bytes.size(), data.begin());
	blob->set(unique_ptr<JsonXBlobData>(&data));
}

void JsonXValue::setBlob(const uint8_t* pb, size_t cb) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	JsonXBlobData data{JsonXBlobData(cb)};
	uninitialized_copy(pb, pb + cb, data.begin());
	blob->set(unique_ptr<JsonXBlobData>(&data));
}

void JsonXValue::setBlob(const JsonXBlobData& _data) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	JsonXBlobData data{_data};
	blob->set(unique_ptr<JsonXBlobData>(&data));
}

void JsonXValue::appendBlob(std::initializer_list<uint8_t> bytes) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	for (uint8_t byte: bytes) blob->add(byte);
}

void JsonXValue::appendBlob(const uint8_t* pb, size_t cb) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	for (; cb > 0; --cb, ++pb) blob->add(*pb);
}

void JsonXValue::appendBlob(const JsonXBlobData& _data) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	for (uint8_t byte: blob->value()) blob->add(byte);
}

void JsonXValue::insertBlob(unique_ptr<JsonXBlobData>&& data) {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	blob->set(move(data));
}

const JsonXBlobData& JsonXValue::getBlob() const {
	assert(m_value);
	const JsonXBlob* blob = dynamic_cast<const JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	return blob->value();
}

unique_ptr<JsonXBlobData> JsonXValue::extractBlob() {
	assert(m_value);
	JsonXBlob* blob = dynamic_cast<JsonXBlob*>(m_value.get());
	if (!blob) throw invalid_argument("Not blob");
	return blob->extract();
}

void JsonXValue::insertArray(unique_ptr<JsonXArrayData>&& data) {
	assert(m_value);
	JsonXArray* array = dynamic_cast<JsonXArray*>(m_value.get());
	if (!array) throw invalid_argument("Not array");
	return array->set(move(data));
}

const JsonXArrayData& JsonXValue::getArray() const {
	assert(m_value);
	const JsonXArray* array = dynamic_cast<const JsonXArray*>(m_value.get());
	if (!array) throw invalid_argument("Not array");
	return array->value();
}

std::unique_ptr<JsonXArrayData> JsonXValue::extractArray() {
	assert(m_value);
	JsonXArray* array = dynamic_cast<JsonXArray*>(m_value.get());
	if (!array) throw invalid_argument("Not array");
	return array->extract();
}

void JsonXValue::insertObject(unique_ptr<JsonXObjectValue>&& data) {
	assert(m_value);
	JsonXObject* object = dynamic_cast<JsonXObject*>(m_value.get());
	if (!object) throw invalid_argument("Not object");
	return object->set(move(data));
}

const JsonXObjectValue& JsonXValue::getObject() const {
	assert(m_value);
	const JsonXObject* object = dynamic_cast<const JsonXObject*>(m_value.get());
	if (!object) throw invalid_argument("Not object");
	return object->value();
}

std::unique_ptr<JsonXObjectValue> JsonXValue::extractObject() {
	assert(m_value);
	JsonXObject* object = dynamic_cast<JsonXObject*>(m_value.get());
	if (!object) throw invalid_argument("Not object");
	return object->extract();
}

} // end namespace JsonX //
