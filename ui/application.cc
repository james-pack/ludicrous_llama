#include "ui/application.h"

#include <functional>
#include <vector>

#include "glog/logging.h"
#include "ui/ui.h"

namespace pack::ui {

Application* Application::instance_{nullptr};

void log_glfw_error(int error_code, const char* description) {
  LOG(ERROR) << "GLFW Error: " << error_code << " -- " << description;
}

Application::Application() {
  if (instance_ != nullptr) {
    throw std::logic_error("Can only have one active Application at a time.");
  }
  instance_ = this;

  glfwSetErrorCallback(log_glfw_error);
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }
}

Application::~Application() {
  glfwTerminate();
  if (instance_ != this) {
    LOG(ERROR) << "Error on Application destruction. Application not registered as current instance";
  }
  instance_ = nullptr;
}

Application& Application::current() {
  if (instance_ == nullptr) {
    throw std::logic_error("No active Application.");
  }
  return *instance_;
}

void Application::add_event(std::function<void()> event) { event_queue_.emplace_back(std::move(event)); }

void Application::distribute_events() {
  // Note that this clears the event_queue_, since we are using the move constructor.
  std::vector<std::function<void()>> current_events{std::move(event_queue_)};
  for (const auto& event : current_events) {
    event();
  }
}

void Application::iterate_services() {
  for (Service* service : services_) {
    service->do_iteration();
  }
}

void Application::execute() {
  while (!should_stop_) {
    distribute_events();
    iterate_services();

    glfwPollEvents();
  }
}

void Application::shutdown() { should_stop_ = true; }

}  // namespace pack::ui
