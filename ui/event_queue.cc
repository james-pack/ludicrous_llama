#include "ui/event_queue.h"

#include <functional>
#include <vector>

namespace pack::ui {

EventQueue& EventQueue::get() {
  static EventQueue instance{};
  return instance;
}

void EventQueue::add(std::function<void()> fn) {
  EventQueue& instance{get()};
  instance.event_queue_.emplace_back(std::move(fn));
}

void EventQueue::distribute() {
  EventQueue& instance{get()};
  std::vector<std::function<void()>> queue{instance.event_queue_};
  instance.event_queue_.clear();
  for (const auto& entry : queue) {
    entry();
  }
}

}  // namespace pack::ui
