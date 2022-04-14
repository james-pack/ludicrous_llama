#include "position/position.h"

#include <string>

namespace pack::position {

std::string to_string(const Orientation& orientation) {
  using std::to_string;
  return "[" + to_string(orientation.orientation[0]) + ", " + to_string(orientation.orientation[1]) + ", " +
         to_string(orientation.orientation[2]) + "]";
}

std::string to_string(const Position& position) {
  using std::to_string;
  return "[" + to_string(position.position[0]) + ", " + to_string(position.position[1]) + ", " +
         to_string(position.position[2]) + "]";
}

}  // namespace pack::position
