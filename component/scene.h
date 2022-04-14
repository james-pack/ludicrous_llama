#pragma once

#include <string>

#include "component/component.h"

namespace pack::component {

struct Scene {
  Subcomponent::Container root_components{};

  bool operator==(const Scene& rhs) const { return root_components == rhs.root_components; }
};

std::string to_string(const Scene& scene) {
  std::string result{};
  result += "{";
  bool need_comma{false};
  for (const auto& component : scene.root_components) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(component);
    need_comma = true;
  }
  result += "}";
  return result;
}

}  // namespace pack::component
