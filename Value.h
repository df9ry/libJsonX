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

#ifndef JSONXVALUE_H_
#define JSONXVALUE_H_

namespace JsonX {

	class Object;
	class List;

	class Value {
	public:
		List& asList();
		Object& asObject();

	protected:
		Value();
	};

	class List: public Value {
	public:

	protected:
		List();
	};

	class Object : public List {
	public:
	protected:
		Object();
	};

} // end namespace JsonX //

#endif
