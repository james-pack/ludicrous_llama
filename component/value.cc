#include "component/value.h"

#include <stdexcept>
#include <string>

namespace pack::component {

std::string to_string(Type type) {
  using std::to_string;

  switch (type) {
    case Type::UNTYPED:
      return "<untyped>";
    case Type::FLOAT:
      return "float";
    case Type::INTEGER:
      return "integer";
    case Type::STRING:
      return "string";
    case Type::ID:
      return "id";
    default:
      throw std::domain_error(std::string("Type '") + to_string(static_cast<int>(as_index(type))) +
                              "' not implemented in to_string(Type)");
  }
}

std::string to_string(const Value& value) {
  using std::to_string;

  Type type{get_type(value)};
  std::string result{};
  switch (type) {
    case Type::UNTYPED:
      break;
    case Type::FLOAT:
      result += to_string(as_float(value));
      break;
    case Type::INTEGER:
      result += to_string(as_integer(value));
      break;
    case Type::STRING:
      result += "'" + as_string(value) + "'";
      break;
    case Type::ID:
      result += as_id(value);
      break;
    default:
      throw std::domain_error(std::string("Type '") + to_string(static_cast<int>(as_index(type))) +
                              "' not implemented in to_string(Value)");
  }

  result += " (" + to_string(type) + ")";
  return result;
}

}  // namespace pack::component
