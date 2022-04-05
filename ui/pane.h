#pragma once

#include "ui/application.h"
#include "ui/layout.h"

namespace pack::ui {

class Pane {
 private:
  mutable Application::registry_type* registry_{nullptr};
  mutable Application::component_table_type* component_table_{nullptr};

  Layout* layout_{nullptr};

 protected:
  Pane() = default;

  auto& registry() {
    if (registry_ == nullptr) {
      registry_ = &Application::current().registry();
    }
    return *registry_;
  }

  const auto& registry() const {
    if (registry_ == nullptr) {
      registry_ = &Application::current().registry();
    }
    return *registry_;
  }

  auto& component_table() {
    if (component_table_ == nullptr) {
      component_table_ = &Application::current().component_table();
    }
    return *component_table_;
  }

  const auto& component_table() const {
    if (component_table_ == nullptr) {
      component_table_ = &Application::current().component_table();
    }
    return *component_table_;
  }

 public:
  virtual ~Pane() = default;

  virtual void set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
    if (layout_ != nullptr) {
      layout_->set_bounds(lower_left_x, lower_left_y, width, height);
    }
  }

  virtual void render() {
    if (layout_ != nullptr) {
      layout_->render();
    }
  }

  void set_layout(Layout& layout) { layout_ = &layout; }

  const Layout* layout() const { return layout_; }
  Layout* layout() { return layout_; }

  void clear_layout() { layout_ = nullptr; }
};

}  // namespace pack::ui
