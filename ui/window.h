#pragma once

#include <vector>

#include "third_party/glfw/glfw.h"
#include "ui/application.h"
#include "ui/compass_layout.h"
#include "ui/pane.h"

namespace pack::ui {

class Window final : public Service {
 private:
  GLFWwindow* window_;
  int width_;
  int height_;
  CompassLayout layout_{};

  std::vector<Pane*> panes_{};

 public:
  Window();
  ~Window();

  void do_iteration();
  void render();
  void change_size(int width, int height);
  void close();

  // TODO(james): Consider pushing ownership of the Panes to the Window.
  void add_pane(Pane& pane, CompassLayout::Region region, float size = 1.f,
                CompassLayout::Unit size_unit = CompassLayout::Unit::RELATIVE) {
    panes_.push_back(&pane);
    layout_.place(pane, region);
    if (region != CompassLayout::Region::CENTER) {
      layout_.configure(region, size, size_unit);
    }
  }
};

}  // namespace pack::ui
