#pragma once

#include "ui/application.h"
#include "ui/compass_layout.h"
#include "ui/component_pane.h"
#include "ui/demo_pane.h"
#include "ui/light_edit_pane.h"
#include "ui/window.h"

namespace pack::demo {

class ApplicationPane final : public ui::Pane {
  ui::CompassLayout layout_{};
  ui::ComponentPane component_pane_{};
  // ui::GearEditPane gui_pane_{};
  // ui::LightEditPane gui_pane_{};
  ui::DemoPane gui_pane_{};

 public:
  ApplicationPane() {
    set_layout(layout_);
    layout_.place(gui_pane_, ui::CompassLayout::Region::EAST);
    layout_.configure(ui::CompassLayout::Region::EAST, 0.2f, ui::CompassLayout::Unit::RELATIVE);
    layout_.place(component_pane_, ui::CompassLayout::Region::CENTER);
  }
};

class GearsUi final {
  ui::Window window_{};
  ApplicationPane pane_{};

 public:
  void assemble(ui::Application& application) {
    window_.set_pane(pane_);

    application.set_window(window_);
    application.add_service(window_);
  }
};

}  // namespace pack::demo
