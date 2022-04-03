#include "component/render.h"

#include "component/parameter.h"
#include "component/primitive.h"

namespace pack::component {

void PrimitiveRenderer::render(const ParameterBinding::Set& bindings) {
  if (primitive_ != nullptr) {
    bool can_render{is_valid_ && previous_bindings_ == bindings};
    if (!can_render) {
      if (primitive_->can_generate(bindings)) {
        previous_bindings_ = bindings;
        draw_func_ = primitive_->generate(previous_bindings_);
        is_valid_ = true;
        can_render = true;
      }
    }
    if (can_render) {
      draw_func_();
    }
  }
}

}  // namespace pack::component
