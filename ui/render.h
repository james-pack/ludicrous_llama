#pragma once

#include <functional>

#include "component/position.h"
#include "third_party/glfw/glfw.h"
#include "ui/model/gear.h"

namespace pack::ui {

using Render = std::function<void(const ui::model::Gear& component, const component::Position& position,
                                  const component::Orientation& orientation)>;

Render construct_draw_list_renderer(GLint draw_list_id);

}  // namespace pack::ui
