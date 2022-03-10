#include "ui/loop.h"

#include <functional>
#include <vector>

namespace pack::ui {

Loop& Loop::get() {
  static Loop instance{};
  return instance;
}

void Loop::add_event(std::function<void()> fn) {
  Loop& instance{get()};
  instance.event_queue_.emplace_back(std::move(fn));
}

void Loop::distribute_events() {
  Loop& instance{get()};
  std::vector<std::function<void()>> queue{instance.event_queue_};
  instance.event_queue_.clear();
  for (const auto& entry : queue) {
    entry();
  }
}

}  // namespace pack::ui
