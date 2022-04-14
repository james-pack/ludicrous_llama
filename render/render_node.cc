#include "render/render_node.h"

#include <stdexcept>
#include <string>

#include "component/component.h"
#include "component/component_table.h"
#include "third_party/glfw/glfw.h"

namespace pack::render {

using namespace pack::component;

void RenderNode::render(ComponentTable& component_table) const {
  using std::to_string;

  bool nontrivial_orientation = !(component_->orientation.is_identity() || component_->orientation.is_zero());
  bool nonzero_position = component_->position.is_nonzero();
  bool need_basis_change = nontrivial_orientation || nonzero_position;

  if (need_basis_change) {
    glPushMatrix();
    if (nontrivial_orientation) {
      glRotatef(component_->orientation.orientation[0], 1.0, 0.0, 0.0);
      glRotatef(component_->orientation.orientation[1], 0.0, 1.0, 0.0);
      glRotatef(component_->orientation.orientation[2], 0.0, 0.0, 1.0);
    }
    if (nonzero_position) {
      glTranslatef(component_->position.position[0], component_->position.position[1],
                   component_->position.position[2]);
    }
  }

  switch (component_->type) {
    case Subcomponent::Type::PRIMITIVE:
      render_primitive();
      break;
    case Subcomponent::Type::COMPONENT:
      render_child(component_table);
      break;
    default:
      throw std::domain_error("Type '" +
                              to_string(static_cast<std::underlying_type_t<Subcomponent::Type>>(component_->type)) +
                              "' not implemented in RenderNode::render()");
  }

  if (need_basis_change) {
    glPopMatrix();
  }
}

void RenderNode::render_primitive() const {
  bool can_render{is_valid_ && previous_bindings_ == component_->bindings &&
                  previous_material_ == component_->material};
  if (!can_render) {
    if (component_->primitive->can_generate(component_->bindings)) {
      previous_bindings_ = component_->bindings;
      previous_material_ = component_->material;
      draw_func_ = component_->primitive->generate(previous_bindings_, previous_material_);
      is_valid_ = true;
      can_render = true;
    }
  }
  if (can_render) {
    draw_func_();
  }
}

void RenderNode::render_child(ComponentTable& component_table) const {
  const Component& child_component{component_table.template get_existing<Component>(component_->child_id)};
  for (const Subcomponent& subcomponent : child_component.children) {
    const RenderNode* node{component_table.get<RenderNode>(subcomponent.id)};
    if (node == nullptr) {
      node = &component_table.emplace<RenderNode>(subcomponent.id, subcomponent);
    }
    node->render(component_table);
  }
}

}  // namespace pack::render
