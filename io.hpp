#ifndef IO_HPP
#define IO_HPP

#include "jsonx.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace jsonx {

void serialize(std::ostream &os, const std::string &v);
void serialize(std::ostream &os, const std::vector<json> &v);
void serialize(std::ostream &os, const std::map<std::string, json> &v);

} // end namespace jsonx //

#endif // IO_HPP
