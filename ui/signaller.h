#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "ui/application.h"

namespace pack::ui {

// TODO(james): Delete this class and replace it with EnTT's built-in observer mechanism.
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
      Application::current().add_event([signal, &instance, &slot]() { slot(signal, instance); });
    }
  }

  // TODO(james): Redesign to provide a disconnect mechanism.
  /* This doesn't work, since std::function<> doesn't provide operator==().
  void disconnect(SignalT signal, const Slot& slot) {
    for (auto iter = observers_.find(signal); iter != observers_.end(); ++iter) {
      if (iter->first != signal) {
        break;
      }
      const auto& registered_slot = iter->second;
      if (slot == registered_slot) {
        observers_.erase(iter);
        break;
      }
    }
  }
  */

 private:
  std::unordered_multimap<SignalT, Slot> observers_{};
};

}  // namespace pack::ui
