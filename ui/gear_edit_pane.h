#pragma once

#include "ui/pane.h"

namespace pack::ui {

class GearEditPane final : public Pane {
 private:
  int lower_left_x_{};
  int lower_left_y_{};
  int width_{};
  int height_{};

 public:
  void set_bounds(int lower_left_x, int lower_left_y, int width, int height) override;
  void render() override;
};

}  // namespace pack::ui
