#pragma once

#include <functional>
#include <vector>

#include "component/component_table.h"
#include "entt/entity/registry.hpp"

namespace pack::ui {

class Animator;
class Service;
class Window;

class Application final {
 public:
  using registry_type = entt::registry;
  using entity_type = typename registry_type::entity_type;
  using component_table_type = typename component::ComponentTable<entity_type>;

 private:
  static Application* instance_;

  std::vector<std::function<void()>> event_queue_{};
  std::vector<Service*> services_{};
  registry_type registry_{};
  component_table_type component_table_{registry_};
  bool should_stop_{false};

  Animator* animator_{nullptr};
  Window* window_{nullptr};

 public:
  Application();
  ~Application();

  registry_type& registry() { return registry_; };
  component_table_type& component_table() { return component_table_; };

  void add_service(Service& service) { services_.push_back(&service); }

  // TODO(james): Consider redesign of these event methods using signals and slots mechanisms from EnTT.
  void add_event(std::function<void()> fn);

  void execute();
  void shutdown();

  // Visible for testing. These methods allow a single iteration of the event loop. These are generally not called
  // directly, but are called via the execute() method.
  void distribute_events();
  void iterate_services();

  Animator* animator() const { return animator_; }
  void set_animator(Animator& animator) { animator_ = &animator; }

  Window* window() const { return window_; }
  void set_window(Window& window) { window_ = &window; }

  static Application& current();
};

class Service {
 private:
  mutable Application::registry_type* registry_{nullptr};
  mutable Application::component_table_type* component_table_{nullptr};

 protected:
  Service() = default;

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
  virtual ~Service() = default;

  virtual void do_iteration() = 0;
};

}  // namespace pack::ui
