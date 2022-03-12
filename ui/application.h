#pragma once

#include <functional>
#include <vector>

#include "entt/entity/registry.hpp"

namespace pack::ui {

class Service;

class Application final {
 private:
  static Application* instance_;

  std::vector<std::function<void()>> event_queue_{};
  std::vector<Service*> services_{};
  entt::registry registry_{};
  bool should_stop_{false};

 public:
  Application();
  ~Application();

  entt::registry& registry() { return registry_; };

  void add_service(Service& service) { services_.push_back(&service); }

  // TODO(james): Consider redesign of these event methods using signals and slots mechanisms from EnTT.
  void add_event(std::function<void()> fn);

  void execute();
  void shutdown();

  // Visible for testing. These methods allow a single iteration of the event loop. These are generally not called
  // directly, but are called via the execute() method.
  void distribute_events();
  void iterate_services();

  static Application& current();
};

class Service {
 private:
  mutable entt::registry* registry_{nullptr};

 protected:
  Service() = default;

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
  virtual ~Service() = default;

  virtual void do_iteration() = 0;
};

}  // namespace pack::ui
