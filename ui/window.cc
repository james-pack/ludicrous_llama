#include "ui/window.h"

#include <stdexcept>

#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "position/position.h"
#include "proto/proto_utils.h"
#include "render/camera.h"
#include "third_party/glfw/glfw.h"
#include "third_party/imgui/imgui.h"
#include "ui/animator.h"
#include "ui/imgui_framer.h"
#include "ui/pane.h"

namespace pack::ui {

void reshape(GLFWwindow* gl_window, int width, int height) {
  Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(gl_window));
  window.change_size(width, height);
}

/* change view angle, exit upon ESC */
void handle_key(GLFWwindow* window, int k, int s, int action, int mods) {
  if (action != GLFW_PRESS) return;

  if (k == GLFW_KEY_SPACE) {
    Animator* animator = Application::current().animator();
    if (animator != nullptr) {
      animator->toggle_pause();
    }
  } else if (k == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  } else {
    auto cameras = Application::current().registry().view<render::Camera>();
    cameras.each([k, mods](render::Camera& camera) {
      switch (k) {
        case GLFW_KEY_UP:
          camera.orientation.orientation[0] += 5.0;
          break;
        case GLFW_KEY_DOWN:
          camera.orientation.orientation[0] -= 5.0;
          break;
        case GLFW_KEY_LEFT:
          camera.orientation.orientation[1] += 5.0;
          break;
        case GLFW_KEY_RIGHT:
          camera.orientation.orientation[1] -= 5.0;
          break;
        case GLFW_KEY_Z:
          if (mods & GLFW_MOD_SHIFT) {
            camera.orientation.orientation[2] -= 5.0;
          } else {
            camera.orientation.orientation[2] += 5.0;
          }
          break;
        case GLFW_KEY_W:
          camera.position.position[2] += 5.0;
          break;
        case GLFW_KEY_X:
          camera.position.position[2] -= 5.0;
          break;
        default:
          return;
      }
    });
  }
}

Window::Window() {
  glfwWindowHint(GLFW_DEPTH_BITS, 32);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &width_, &height_);
    window_ = glfwCreateWindow(width_, height_, "Gears", monitor, NULL);
    if (!window_) {
      LOG(FATAL) << "Failed to create GLFW window.";
      throw std::runtime_error("Failed to create GLFW window.");
    }
  }

  // Setup Dear ImGui context. Must be done before calling ImGui::GetIO().
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  glfwSetWindowUserPointer(window_, this);

  // Set callback functions
  glfwSetFramebufferSizeCallback(window_, reshape);
  glfwSetKeyCallback(window_, handle_key);

  glfwMakeContextCurrent(window_);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Render backends
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(determine_glsl_version());

  glEnable(GL_RESCALE_NORMAL);
  glEnable(GL_LINE_SMOOTH);
}

Window::~Window() {
  // Cleanup and shutdown.
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Window::do_iteration() { render(); }

void Window::render() {
  if (glfwWindowShouldClose(window_)) {
    close();
    return;
  }

  {
    // Note that this scope exists to ensure that the framer goes out of scope before the buffer swap below.
    ImGuiFramer framer{};

    // Since the component render overwrites the full window, we have to render the gui afterwards.
    // If we can limit the component drawing to a particular region, then we may be able to optimize GUI redraws only if
    // changed/dirty.

    // Draw components.
    if (pane_ != nullptr) {
      pane_->render();
    }
  }

  // Swap buffers
  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void Window::close() {
  // TODO(james): Modify to allow for multiple windows. Closing one window shouldn't shutdown the entire application.
  Application::current().shutdown();
}

void Window::change_size(int width, int height) {
  width_ = width;
  height_ = height;
  if (pane_ != nullptr) {
    pane_->set_bounds(0, 0, width, height);
  }
}

}  // namespace pack::ui
