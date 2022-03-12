#pragma once

#include "ui/application.h"
#include "ui/component_pane.h"
#include "ui/demo_pane.h"
#include "ui/window.h"

namespace pack::demo {

class GearsUi final {
  ui::Window window_{};
  ui::ComponentPane component_pane_{};
  ui::DemoPane gui_pane_{};

 public:
  void assemble(ui::Application& application) {
    window_.add_pane(component_pane_);
    window_.add_pane(gui_pane_);

    application.add_service(window_);
  }
};

}  // namespace pack::demo
