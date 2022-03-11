#include "ui/window.h"

#include <stdexcept>

#include "color/materials.h"
#include "component/gear.h"
#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "proto/proto_utils.h"
#include "ui/camera.h"
#include "ui/lighting_im_render.h"
#include "ui/lighting_model.h"
#include "ui/render.h"
#include "ui/ui.h"

namespace pack::ui {

using namespace pack::color;
using namespace pack::component;
using namespace pack::proto;

void reshape(GLFWwindow* gl_window, int width, int height) {
  Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(gl_window));
  window.change_size(width, height);
}

/* change view angle, exit upon ESC */
void handle_key(GLFWwindow* window, int k, int s, int action, int mods) {
  if (action != GLFW_PRESS) return;

  if (k == GLFW_KEY_SPACE) {
    // application.animator.toggle_pause();
  } else if (k == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  } else {
    auto cameras = Application::current().registry().view<Camera, Position, Orientation>();
    cameras.each([k, mods](Camera& camera, Position& position, Orientation& orientation) {
      switch (k) {
        case GLFW_KEY_UP:
          orientation.set_rot_x(orientation.rot_x() + 5.0);
          break;
        case GLFW_KEY_DOWN:
          orientation.set_rot_x(orientation.rot_x() - 5.0);
          break;
        case GLFW_KEY_LEFT:
          orientation.set_rot_y(orientation.rot_y() + 5.0);
          break;
        case GLFW_KEY_RIGHT:
          orientation.set_rot_y(orientation.rot_y() - 5.0);
          break;
        case GLFW_KEY_Z:
          if (mods & GLFW_MOD_SHIFT) {
            orientation.set_rot_z(orientation.rot_z() - 5.0);
          } else {
            orientation.set_rot_z(orientation.rot_z() + 5.0);
          }
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

  Application& app = Application::current();
  {
    const auto camera = app.registry().create();
    app.registry().emplace<Camera>(camera, Camera{});
    app.registry().emplace<Position>(camera, Position{});
    app.registry().emplace<Orientation>(camera, Orientation{});
  }

  {
    Gears gears = load_text_proto<Gears>("demo/trivial_demo_gears.pb.txt");
    DLOG(INFO) << "Gears loaded:\n" << gears.DebugString();

    for (Gear gear : gears.gear()) {
      Material* material = gear.mutable_material();
      Materials::pack_colors(material);
      const auto gear_id = app.registry().create();
      GLint draw_list_id = build_gear(gear);
      app.registry().emplace<Render>(gear_id, construct_draw_list_renderer(draw_list_id));
      app.registry().emplace<Animate>(gear_id, construct_gear_animator(gear));
      app.registry().emplace<Gear>(gear_id, std::move(gear));
      Position position{};
      position.mutable_float_values()->set_x(-3.1f);
      position.mutable_float_values()->set_y(4.2f);
      app.registry().emplace<Position>(gear_id, position);
      app.registry().emplace<Orientation>(gear_id, Orientation{});
    }
  }

  const auto ui_models = app.registry().create();
  {
    app.registry().emplace<LightingModel>(ui_models, LightingModel{});
    LightingModel& model = app.registry().get<LightingModel>(ui_models);
    model.connect(LightingModelSignal::POSITION_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::COLOR_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::ENABLED_UPDATE, lighting_im_signal_render);
    model.connect(LightingModelSignal::RESET_UPDATE, lighting_im_signal_render);
    model.set_lighting_configuration_path("demo/lighting_configuration.pb.txt");
    model.load();
  }

  glEnable(GL_RESCALE_NORMAL);
  glEnable(GL_LINE_SMOOTH);
}

Window::~Window() {
  // Cleanup and shutdown.
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Window::do_iteration() {
  if (glfwWindowShouldClose(window_)) {
    close();
    return;
  }

  // Since the component_draw overwrites the full window, we have to draw the gui afterwards.
  // If we can limit the component drawing to a particular region, then we may be able to optimize GUI redraws only if
  // changed/dirty.

  // Draw components.
  // TODO(james): Extract this block to a ComponentPane or similar.
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  const auto cameras = registry().view<const Camera, const Position, const Orientation>();
  cameras.each([](const Camera& camera, const Position& position, const Orientation& orientation) {
    glRotatef(orientation.rot_x(), 1.0, 0.0, 0.0);
    glRotatef(orientation.rot_y(), 0.0, 1.0, 0.0);
    glRotatef(orientation.rot_z(), 0.0, 0.0, 1.0);
    glTranslatef(position.float_values().x(), position.float_values().y(), position.float_values().z());
  });

  const auto gears = registry().view<Render, Gear, Position, Orientation>();
  gears.each([](const Render& render, const Gear& component, const Position& position,
                const Orientation& orientation) {  //
    render(component, position, orientation);
  });

  glPopMatrix();

  // Draw gui.
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  bool show_demo_window{false};
  ImGui::ShowDemoWindow(&show_demo_window);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

  // TODO(james): Modify to manage a component pane(s) separately from UI panes.
  GLfloat aspect = (GLfloat)height / (GLfloat)width;
  GLfloat znear = 2.f;
  GLfloat zfar = 50.f;
  GLfloat xmax = znear / 2.f;

  glViewport(0, 0, width_, height_);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xmax, xmax, -xmax * aspect, xmax * aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, (znear - zfar) / 2.f);
}

}  // namespace pack::ui
