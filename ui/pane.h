#pragma once

#include "entt/entity/registry.hpp"
#include "ui/application.h"

namespace pack::ui {

class Pane {
 private:
  mutable entt::registry* registry_{nullptr};

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

  virtual void set_bounds(int lower_left_x, int lower_left_y, int width, int height) {}
  virtual void render() = 0;
};

}  // namespace pack::ui
