#pragma once

#include "entt/entity/registry.hpp"
#include "ui/application.h"
#include "ui/layout.h"

namespace pack::ui {

class Pane {
 private:
  mutable entt::registry* registry_{nullptr};

  Layout* layout_{nullptr};

 protected:
  Pane() = default;

  entt::registry& registry() {
    if (registry_ == nullptr) {
      registry_ = &Application::current().registry();
    }
    return *registry_;
  }

  const entt::registry& registry() const {
    if (registry_ == nullptr) {
      registry_ = &Application::current().registry();
    }
    return *registry_;
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
