#pragma once

#include <functional>

#include "position/position.h"
#include "third_party/glfw/glfw.h"
#include "ui/model/gear.h"

namespace pack::ui {

using Render = std::function<void(const ui::model::Gear& component, const position::Position& position,
                                  const position::Orientation& orientation)>;

Render construct_draw_list_renderer(GLint draw_list_id);

}  // namespace pack::ui
