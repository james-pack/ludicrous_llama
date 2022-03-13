#pragma once

#include <vector>

#include "ui/compass_layout.h"
#include "ui/pane.h"

namespace pack::ui {

// TODO(james): Refactor this class into the Pane class. All Panes should accept a layout, and that should be the only
// mechanism to add child panes, widgets, etc.
class LayoutPane : public Pane {
 private:
  CompassLayout layout_{};
  std::vector<Pane*> panes_{};

 public:
  virtual ~LayoutPane() = default;

  void render() override;

  void add_pane(Pane& pane, CompassLayout::Region region, float size = 1.f,
                CompassLayout::Unit size_unit = CompassLayout::Unit::RELATIVE) {
    panes_.push_back(&pane);
    layout_.place(pane, region);
    if (region != CompassLayout::Region::CENTER) {
      layout_.configure(region, size, size_unit);
    }
  }

  void set_bounds(int lower_left_x, int lower_left_y, int width, int height) override;
};

}  // namespace pack::ui
