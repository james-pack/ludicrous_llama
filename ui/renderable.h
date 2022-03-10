#pragma once

#include <functional>

#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "ui/ui.h"

namespace pack::ui {

using Renderable = std::function<void(const component::Gear& component, const component::Position& position,
                                      const component::Orientation& orientation)>;

Renderable construct_draw_list_renderable(GLint draw_list_id);

}  // namespace pack::ui
