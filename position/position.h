#pragma once

#include <string>

namespace pack::position {

struct Orientation final {
  // Orientation in 3D space as Euler angles in the order of [rot_x, rot_y, rot_z].
  float orientation[3]{};

  bool operator==(const Orientation& rhs) const {
    return orientation[0] == rhs.orientation[0] &&  //
           orientation[1] == rhs.orientation[1] &&  //
           orientation[2] == rhs.orientation[2];
  }

  bool is_nonzero() const {
    return orientation[0] != 0.f ||  //
           orientation[1] != 0.f ||  //
           orientation[2] != 0.f;
  }
};

std::string to_string(const Orientation& orientation);

struct Position final {
  // Position in 4D space (homogeneous object coordinates) as an array in the order [x, y, z, w].
  float position[4]{};

  bool operator==(const Position& rhs) const {
    return position[0] == rhs.position[0] &&  //
           position[1] == rhs.position[1] &&  //
           position[2] == rhs.position[2] &&  //
           position[3] == rhs.position[3];
  }

  bool is_nonzero() const {
    return position[0] != 0.f ||  //
           position[1] != 0.f ||  //
           position[2] != 0.f ||  //
           position[3] != 0.f;
  }
};

std::string to_string(const Position& position);

}  // namespace pack::position
