#pragma once

#include "entt/entity/observer.hpp"
#include "ui/pane.h"

namespace pack::ui {

class ComponentPane final : public Pane {
 private:
  entt::observer camera_observer_;
  entt::observer component_observer_;
  entt::observer lighting_observer_;
  bool requires_redraw_{false};

 public:
  ComponentPane();

  void set_bounds(int lower_left_x, int lower_left_y, int width, int height) override;
  void render() override;
};

}  // namespace pack::ui
