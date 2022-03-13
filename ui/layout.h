#pragma once

#include <tuple>

namespace pack::ui {

/**
 * Layouts own the rendering of child widgets. Specific subclasses of the Layout class will manage the child widgets
 * differently and will have their own APIs for managing the membership of children and their placement.
 */
class Layout {
 public:
  virtual ~Layout() = default;

  virtual void set_bounds(int origin_x, int origin_y, int width, int height) = 0;

  virtual void render() = 0;
};

}  // namespace pack::ui
