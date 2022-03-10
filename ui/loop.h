#pragma once

#include <functional>
#include <vector>

namespace pack::ui {

// TODO(james): Consider redesign using signals and slots mechanisms from EnTT.
class Loop final {
 private:
  std::vector<std::function<void()>> event_queue_{};

 public:
  static Loop& get();

  static void distribute();

  static void add(std::function<void()> fn);
};

}  // namespace pack::ui
