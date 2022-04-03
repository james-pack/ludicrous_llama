#pragma once

#include "entt/entity/registry.hpp"
#include "ui/application.h"

namespace pack::ui {

// TODO(james): Redesign this concept to use time-based expressions and handle changes to create animations during
// expression evaluation.
class Animator final : public Service {
 private:
  bool is_paused_{false};

 public:
  void do_iteration() override;
  void animate();

  bool is_paused() const { return is_paused_; }

  void pause() { is_paused_ = true; }

  void unpause() { is_paused_ = false; }

  void toggle_pause() {
    if (is_paused_) {
      is_paused_ = false;
    } else {
      is_paused_ = true;
    }
  }
};

}  // namespace pack::ui
