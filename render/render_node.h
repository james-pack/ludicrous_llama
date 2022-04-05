#pragma once

#include "component/component.h"
#include "component/component_table.h"
#include "component/parameter.h"
#include "component/primitive.h"
#include "third_party/glfw/glfw.h"

namespace pack::render {

class RenderNode final {
 private:
  const component::Component* component_;

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

  template <typename EntityT>
  void render_children(const component::ComponentTable<EntityT>& component_table) const {
    for (const auto& child : component_->children) {
      const RenderNode& subrender{component_table.template get_existing<RenderNode>(child.id)};
      bool nonzero_orientation = child.orientation.is_nonzero();
      bool nonzero_position = child.position.is_nonzero();
      bool need_matrix = nonzero_orientation || nonzero_position;

      if (need_matrix) {
        glPushMatrix();
        if (nonzero_orientation) {
          glRotatef(child.orientation.orientation[0], 1.0, 0.0, 0.0);
          glRotatef(child.orientation.orientation[1], 0.0, 1.0, 0.0);
          glRotatef(child.orientation.orientation[2], 0.0, 0.0, 1.0);
        }
        if (nonzero_position) {
          glTranslatef(child.position.position[0], child.position.position[1], child.position.position[2]);
        }
      }

      subrender.render(component_table);

      if (need_matrix) {
        glPopMatrix();
      }
    }
  }

 public:
  RenderNode(const component::Component& component) : component_(&component) {}

  RenderNode(const RenderNode& rhs) = default;
  RenderNode(RenderNode&& rhs) = default;
  RenderNode& operator=(const RenderNode& rhs) = default;
  RenderNode& operator=(RenderNode&& rhs) = default;

  template <typename EntityT>
  void render(const component::ComponentTable<EntityT>& component_table) const {
    if (component_->primitive != nullptr) {
      render_primitive();
    }
    if (!component_->children.empty()) {
      render_children(component_table);
    }
  }
};

}  // namespace pack::render
