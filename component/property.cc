#include "component/property.h"

#include <string>

namespace pack::component {

std::string to_string(const Property& property) {
  std::string result{};
  result += "name: " + property.name;
  result += ", ";

  result += "value: " + to_string(property.value);

  return result;
}

std::string to_string(const Property::Set& properties) {
  std::string result{"{"};
  bool need_comma{false};
  for (const auto& property : properties) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(property);
    need_comma = true;
  }
  result += "}";
  return result;
}

}  // namespace pack::component
