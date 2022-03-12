#pragma once

#include "ui/pane.h"

namespace pack::ui {

class ComponentPane final : public Pane {
 public:
  void set_bounds(int lower_left_x, int lower_left_y, int width, int height) override;
  void render() override;
};

}  // namespace pack::ui
