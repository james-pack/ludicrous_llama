#include "render/scene.h"

#include <string>

namespace pack::render {

std::string to_string(const Scene& scene) {
  std::string result{};
  result += "root_components: {";
  bool need_comma{false};
  for (const auto& component : scene.root_components) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(component);
    need_comma = true;
  }
  result += "},\n";

  result += "components: {";
  need_comma = false;
  for (const auto& component : scene.components) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(component);
    need_comma = true;
  }
  result += "}";

  result += "cameras: {";
  need_comma = false;
  for (const auto& camera : scene.cameras) {
    if (need_comma) {
      result += ", ";
    }
    result += to_string(camera);
    need_comma = true;
  }
  result += "}";

  return result;
}

}  // namespace pack::render
