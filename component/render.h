#pragma once

#include "component/parameter.h"
#include "component/primitive.h"

namespace pack::component {

class PrimitiveRenderer final {
 private:
  Primitive* primitive_;
  ParameterBinding::Set previous_bindings_{};
  DrawFunc draw_func_{};
  bool is_valid_{false};

 public:
  PrimitiveRenderer(const Primitive& primitive) : primitive_(&primitive) {}

  bool render(const ParameterBinding::Set& bindings);
};

}  // namespace pack::component
