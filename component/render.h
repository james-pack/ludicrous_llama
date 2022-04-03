#pragma once

#include "component/parameter.h"
#include "component/primitive.h"

namespace pack::component {

class PrimitiveRenderer final {
 private:
  const Primitive* primitive_;
  ParameterBinding::Set previous_bindings_{};
  DrawFunc draw_func_{};
  bool is_valid_{false};

 public:
  PrimitiveRenderer() : primitive_(nullptr) {}
  PrimitiveRenderer(const Primitive& primitive) : primitive_(&primitive) {}
  PrimitiveRenderer(const PrimitiveRenderer& rhs) = default;
  PrimitiveRenderer(PrimitiveRenderer&& rhs) = default;
  PrimitiveRenderer& operator=(const PrimitiveRenderer& rhs) = default;
  PrimitiveRenderer& operator=(PrimitiveRenderer&& rhs) = default;

  void render(const ParameterBinding::Set& bindings);
};

}  // namespace pack::component
