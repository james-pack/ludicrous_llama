#pragma once

#include "ui/application.h"
#include "ui/compass_layout.h"
#include "ui/component_pane.h"
#include "ui/layout_pane.h"
#include "ui/light_edit_pane.h"
#include "ui/window.h"

namespace pack::demo {

class ApplicationPane final : public ui::LayoutPane {
  ui::ComponentPane component_pane_{};
  ui::LightEditPane gui_pane_{};

 public:
  ApplicationPane() {
    add_pane(gui_pane_, ui::CompassLayout::Region::EAST, 0.2f, ui::CompassLayout::Unit::RELATIVE);
    add_pane(component_pane_, ui::CompassLayout::Region::CENTER);
  }
};

class GearsUi final {
  ui::Window window_{};
  ApplicationPane pane_{};

 public:
  void assemble(ui::Application& application) {
    window_.add_pane(pane_, ui::CompassLayout::Region::CENTER);

    application.add_service(window_);
  }
};

}  // namespace pack::demo
