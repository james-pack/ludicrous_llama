#include "component/component.h"

#include <string>

namespace pack::component {

std::string to_string(const Component& component) {
  std::string result{};
  result += "id: " + component.id.as_string();
  result += ", ";

  result += "name: '" + component.name + "'";
  result += ", ";

  result += "primitive: ";
  if (component.primitive) {
    result += component.primitive->name();
  } else {
    result += "<null>";
  }
  result += ", ";

  result += to_string(component.bindings);
  result += ", ";

  // TODO(james): Add remaining fields.

  return result;
}

}  // namespace pack::component
