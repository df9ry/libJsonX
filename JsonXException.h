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

#ifndef JSONXEXCEPTION_H_
#define JSONXEXCEPTION_H_

#include <exception>
#include <string>

namespace JsonX {

class JsonXException: public std::runtime_error {
public:
	/**
	 * This package uses JsonXException to be specific.
	 * @param what Error text, as usual
	 */
	JsonXException(const std::string& what):
		std::runtime_error{what}, m_inner{nullptr},
		m_shat{std::runtime_error::what()},
		m_what{m_shat.c_str()} {};

	/**
	 * This package uses JsonXException to be specific.
	 * @param what Error text, as usual
	 * @param inner Inner exception
	 */
	JsonXException(
			const std::string& what,
			const std::exception& inner):
		std::runtime_error{what}, m_inner{&inner},
		m_shat{std::string(std::runtime_error::what()) + ": " +
			inner.what()}, m_what{m_shat.c_str()} {};

	/**
	 * Destructor.
	 */
	~JsonXException() {};

	virtual const char* what() const noexcept { return m_what; }

	/**
	 * Get the inner exception.
	 * @return the inner exception, if one.
	 */
	const std::exception* getInner() const noexcept { return m_inner; }

private:
	const std::exception* m_inner;
	const std::string     m_shat;
	const char*           m_what;
};

} /* namespace JsonX */

#endif /* JSONXEXCEPTION_H_ */
