#pragma once

#include <functional>

#include "third_party/glfw/glfw.h"
#include "ui/model/gear.h"
#include "ui/model/position.h"

namespace pack::ui {

using Render = std::function<void(const ui::model::Gear& component, const ui::model::Position& position,
                                  const ui::model::Orientation& orientation)>;

Render construct_draw_list_renderer(GLint draw_list_id);

}  // namespace pack::ui
