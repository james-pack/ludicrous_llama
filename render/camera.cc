#include "render/camera.h"

#include <string>

namespace pack::render {

std::string to_string(const Camera& camera) {
  std::string result{};
  result += "{";
  result += "name: " + camera.name;
  result += ", ";
  result += "position: " + to_string(camera.position);
  result += ", ";
  result += "orientation: " + to_string(camera.orientation);
  result += "}";
  return result;
}

}  // namespace pack::render
