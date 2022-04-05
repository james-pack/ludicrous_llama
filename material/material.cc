#include "material/material.h"

#include <string>

namespace pack::material {

std::string to_string(const Material& material) {
  std::string result{};
  result += "id: " + material.id;
  result += ", ";

  result += "ambient: " + to_string(material.ambient);
  result += ", ";

  result += "diffuse: " + to_string(material.diffuse);
  result += ", ";

  result += "specular: " + to_string(material.specular);
  result += ", ";

  result += "shininess: " + std::to_string(material.shininess);
  return result;
}

}  // namespace pack::material
