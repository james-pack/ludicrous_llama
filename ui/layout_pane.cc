#include "ui/layout_pane.h"

#include "ui/pane.h"

namespace pack::ui {

void LayoutPane::render() {
  for (Pane* pane : panes_) {
    pane->render();
  }
}

void LayoutPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  layout_.set_bounds(lower_left_x, lower_left_y, width, height);

  for (Pane* pane : panes_) {
    auto [origin_x, origin_y, pane_width, pane_height] = layout_.compute_bounds(*pane);
    pane->set_bounds(origin_x, origin_y, pane_width, pane_height);
  }
}

}  // namespace pack::ui
