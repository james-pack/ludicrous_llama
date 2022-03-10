#pragma once

#include "lighting/light.pb.h"

namespace pack::lighting {

class Lights final {
 public:
  static Light as_packed(const Light& light);
};

}  // namespace pack::lighting
