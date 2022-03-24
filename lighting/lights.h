#pragma once

#include "serialization/light.pb.h"

namespace pack::lighting {

class Lights final {
 public:
  static serialization::Light as_packed(const serialization::Light& light);
};

}  // namespace pack::lighting
