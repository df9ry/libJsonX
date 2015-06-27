/*
    Project FreeAX25.TCPServer
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

#ifndef JSONXTYPE_H_
#define JSONXTYPE_H_

#include <cstdint>

namespace JsonX {

	/**
	 * Type of a value.
	 */
	enum class Type : std::uint8_t {
		NONE, SIGNED, UNSIGNED, BOOL, FLOAT,
		BLOB, STRING, LIST, OBJECT
	};

} // end namespace JsonX //

#endif /* JSONXTYPE_H_ */
