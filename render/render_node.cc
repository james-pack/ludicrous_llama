#include "render/render_node.h"

#include "component/component.h"

namespace pack::render {

void RenderNode::render_primitive() const {
  bool can_render{is_valid_ && previous_bindings_ == component_->bindings};
  if (!can_render) {
    if (component_->primitive->can_generate(component_->bindings)) {
      previous_bindings_ = component_->bindings;
      draw_func_ = component_->primitive->generate(previous_bindings_);
      is_valid_ = true;
      can_render = true;
    }
  }
  if (can_render) {
    draw_func_();
  }
}

}  // namespace pack::render
