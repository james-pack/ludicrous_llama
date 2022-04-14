#pragma once

#include "component/component.h"
#include "component/component_table.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "material/material.h"

namespace pack::render {

class RenderNode final {
 private:
  const component::Subcomponent* component_;

  // Cached rendering details.
  // The set of previous parameter bindings. If these change, we need to regenerate the draw function.
  mutable component::ParameterBinding::Set previous_bindings_{};

  // The material used to render the component. Again, if this changes, we need to regenerate the draw function.
  mutable material::Material previous_material_{};

  // Draw function. Calling this renders the component. Note that the draw function may contain cached information or
  // may manage objects on the GPU (VBO, PBO, draw lists, etc.)
  mutable component::DrawFunc draw_func_{};

  // Allows for the cached objects to be marked as valid or invalid. If they are invalid, they need to be recalculated.
  mutable bool is_valid_{false};

  void render_primitive() const;
  void render_child(component::ComponentTable& component_table) const;

 public:
  RenderNode(const component::Subcomponent& component) : component_(&component) {}

  RenderNode(const RenderNode& rhs) = default;
  RenderNode(RenderNode&& rhs) = default;
  RenderNode& operator=(const RenderNode& rhs) = default;
  RenderNode& operator=(RenderNode&& rhs) = default;

  void render(component::ComponentTable& component_table) const;
};

}  // namespace pack::render
