#include "component/component.h"

#include <string>

namespace pack::component {

std::string to_string(Subcomponent::Type type) {
  using std::to_string;

  switch (type) {
    case Subcomponent::Type::UNTYPED:
      return "<untyped>";
    case Subcomponent::Type::PRIMITIVE:
      return "primitive";
    case Subcomponent::Type::COMPONENT:
      return "component";
    default:
      throw std::domain_error(std::string("Type '") +
                              to_string(static_cast<std::underlying_type_t<Subcomponent::Type>>(type)) +
                              "' not implemented in to_string(Type)");
  }
}

std::string to_string(const Subcomponent& subcomponent) {
  std::string result{};
  result += "type: " + to_string(subcomponent.type);

  if (subcomponent.type == Subcomponent::Type::PRIMITIVE) {
    result += "primitive: ";
    result += subcomponent.primitive->name();
    result += ", ";
    result += "material: " + to_string(subcomponent.material);
    result += ", ";
  }

  if (subcomponent.type == Subcomponent::Type::COMPONENT) {
    result += "child_id: " + to_string(subcomponent.child_id);
    result += ", ";
  }

  result += "position: " + to_string(subcomponent.position);
  result += ", ";

  result += "orientation: " + to_string(subcomponent.orientation);
  result += ", ";

  result += to_string(subcomponent.bindings);

  return result;
}

std::string to_string(const Subcomponent::Container& subcomponents) {
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

  result += "children: " + to_string(component.children);
  result += ", ";

  result += "parameters: " + to_string(component.parameters);
  result += ", ";

  result += "properties: " + to_string(component.properties);

  return result;
}

}  // namespace pack::component
