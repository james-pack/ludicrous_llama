#include "material/color.h"

#include <string>

namespace pack::material {

std::string to_string(const Color& color) {
  using std::to_string;
  return "[" + to_string(color.values[0]) + ", " + to_string(color.values[1]) + ", " + to_string(color.values[2]) +
         ", " + to_string(color.values[3]) + "]";
}

}  // namespace pack::material
