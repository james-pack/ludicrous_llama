#include "component/component.h"

#include <string>

namespace pack::component {

std::string to_string(const Subcomponent& subcomponent) {
  std::string result{};
  result += "id: " + to_string(subcomponent.id);
  result += ", ";

  result += "position: " + to_string(subcomponent.position);
  result += ", ";

  result += "orientation: " + to_string(subcomponent.orientation);
  result += ", ";

  result += to_string(subcomponent.bindings);

  return result;
}

std::string to_string(const Subcomponent::Set& subcomponents) {
  std::string result{"{"};
  bool need_comma{false};
  for (const auto& subcomponent : subcomponents) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(subcomponent);
    need_comma = true;
  }
  result += "}";
  return result;
}

std::string to_string(const Component& component) {
  std::string result{};
  result += "id: " + to_string(component.id);
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

  result += "bindings: " + to_string(component.bindings);
  result += ", ";

  result += "material: " + to_string(component.material);
  result += ", ";

  result += "children: " + to_string(component.children);
  result += ", ";

  result += "parameters: " + to_string(component.parameters);
  result += ", ";

  result += "properties: " + to_string(component.properties);

  return result;
}

}  // namespace pack::component
