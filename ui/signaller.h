#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "ui/event_queue.h"

namespace pack::ui {

template <typename SubT, typename SignalT>
class Signaller {
 public:
  using Slot = std::function<void(SignalT, const SubT&)>;

  virtual ~Signaller() = default;

  void connect(SignalT signal, Slot slot) { observers_.emplace(signal, slot); }

  void signal(SignalT signal, const SubT& instance) const {
    for (auto iter = observers_.find(signal); iter != observers_.end(); ++iter) {
      if (iter->first != signal) {
        break;
      }
      const auto& slot = iter->second;
      EventQueue::add([signal, &instance, &slot]() { slot(signal, instance); });
    }
  }

  void disconnect(SignalT signal, const Slot& slot) {}

 private:
  std::unordered_multimap<SignalT, Slot> observers_{};
};

}  // namespace pack::ui