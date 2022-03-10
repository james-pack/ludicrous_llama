// Based on glFW example named gears.c. Modified for C++.
//
// Original header comment:
/*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Command line options:
 *    -info      print GL implementation information
 *    -exit      automatically exit after 30 seconds
 *
 *
 * Brian Paul
 *
 *
 * Marcus Geelnard:
 *   - Conversion to GLFW
 *   - Time based rendering (frame rate independent)
 *   - Slightly modified camera that should work better for stereo viewing
 *
 *
 * Camilla Löwy:
 *   - Removed FPS counter (this is not a benchmark)
 *   - Added a few comments
 *   - Enabled vsync
 */

// TODO(james): Extract to a separate file to manage math constants. Configure to use C++20 std::numbers::pi when
// available. Feature testing macro: __cpp_lib_math_constants.
#if defined(_MSC_VER)
// Make MS math.h define M_PI
#define _USE_MATH_DEFINES
#endif

// This group of headers is order dependent and must be included before other headers.
#define GLAD_GL_IMPLEMENTATION
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
// End of order dependent headers.

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "color/color.pb.h"
#include "color/colors.h"
#include "color/material.pb.h"
#include "color/materials.h"
#include "component/position.pb.h"
#include "entt/entity/registry.hpp"
#include "gear/gear.h"
#include "gear/gear.pb.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "lighting/light.pb.h"
#include "lighting/lights.h"
#include "proto/proto_utils.h"

namespace pack::gear {

using color::Material;
using component::Orientation;
using component::Position;
using lighting::Light;

struct ComponentVisibility final {
  // Visibility of the component as controlled by one or more UI elements, not the intrinsic alpha channel of the
  // component's color. That alpha channel should be multiplied by this visibility to determine the actual opacity.
  // Similiar approaches should be taken for more complex material modelling.
  GLfloat visibility{1.f};
  // Visibility switch for the component. Note that this switch allows the visibility above to maintained.
  GLboolean is_visible{true};
};

struct PaneLayout final {
  // Offset wrt parent layout.
  GLint lower_left_x{0};
  GLint lower_left_y{0};

  // -1 means use full available size in that direction.
  GLint width{-1};
  GLint height{-1};
};

struct GLId final {
  GLint gl_id;
};

struct SceneParameters final {
  Position scene_position{};
  Orientation scene_orientation{};
  GLfloat gear_rotation_angle{0.f};
  bool animation_paused{false};
};

class Application final {
 public:
  Application(ImGuiIO& io_) : io(io_) {}
  entt::registry registry{};
  ImGuiIO& io;

  // TODO(james): Sloppy. Move to ECS.
  entt::registry::entity_type scene_parameters{};
  PaneLayout viewport{};
};

/* OpenGL draw function & timing */
static void component_draw(const entt::registry& registry) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  const auto scene_parameters = registry.view<const SceneParameters>();
  scene_parameters.each([](const auto& parameters) {
    glRotatef(parameters.scene_orientation.rot_x(), 1.0, 0.0, 0.0);
    glRotatef(parameters.scene_orientation.rot_y(), 0.0, 1.0, 0.0);
    glRotatef(parameters.scene_orientation.rot_z(), 0.0, 0.0, 1.0);
    glTranslatef(parameters.scene_position.float_values().x(), parameters.scene_position.float_values().y(),
                 parameters.scene_position.float_values().z());
  });

  const auto gears = registry.view<GLId, Position, Orientation>();
  gears.each([](const GLId& id, const Position& position, const Orientation& orientation) {
    glPushMatrix();
    glTranslatef(position.float_values().x(), position.float_values().y(), position.float_values().z());
    glRotatef(orientation.rot_x(), 1.0, 0.0, 0.0);
    glRotatef(orientation.rot_y(), 0.0, 1.0, 0.0);
    glRotatef(orientation.rot_z(), 0.0, 0.0, 1.0);
    glCallList(id.gl_id);
    glPopMatrix();
  });

  glPopMatrix();
}

/* update animation parameters */
static void animate(Application* app) {
  SceneParameters& params = app->registry.get<SceneParameters>(app->scene_parameters);
  if (!params.animation_paused) {
    params.gear_rotation_angle = 100.f * (float)glfwGetTime();

    auto gears = app->registry.view<Gear, Orientation>();
    gears.each([&params](const Gear& gear, Orientation& orientation) {
      orientation.set_rot_z(params.gear_rotation_angle * gear.angle_coefficient() + gear.phase());
    });
  }
}

/* change view angle, exit upon ESC */
void key(GLFWwindow* window, int k, int s, int action, int mods) {
  if (action != GLFW_PRESS) return;

  Application& application = *static_cast<Application*>(glfwGetWindowUserPointer(window));
  SceneParameters& params = application.registry.get<SceneParameters>(application.scene_parameters);
  switch (k) {
    case GLFW_KEY_Z:
      if (mods & GLFW_MOD_SHIFT)
        params.scene_orientation.set_rot_z(params.scene_orientation.rot_z() - 5.0);
      else
        params.scene_orientation.set_rot_z(params.scene_orientation.rot_z() + 5.0);
      break;
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_UP:
      params.scene_orientation.set_rot_x(params.scene_orientation.rot_x() + 5.0);
      break;
    case GLFW_KEY_DOWN:
      params.scene_orientation.set_rot_x(params.scene_orientation.rot_x() - 5.0);
      break;
    case GLFW_KEY_LEFT:
      params.scene_orientation.set_rot_y(params.scene_orientation.rot_y() + 5.0);
      break;
    case GLFW_KEY_RIGHT:
      params.scene_orientation.set_rot_y(params.scene_orientation.rot_y() - 5.0);
      break;
    case GLFW_KEY_SPACE:
      if (params.animation_paused) {
        params.animation_paused = false;
      } else {
        params.animation_paused = true;
      }
      break;
    default:
      return;
  }
}

// Window resize, possibly due to monitor resolution change.
void reshape(GLFWwindow* window, int width, int height) {
  Application& application = *static_cast<Application*>(glfwGetWindowUserPointer(window));
  application.viewport.width = width;
  application.viewport.height = height;

  // TODO(james): Modify to manage a component pane(s) separately from UI panes.
  GLfloat aspect = (GLfloat)height / (GLfloat)width;
  GLfloat znear = 2.f;
  GLfloat zfar = 50.f;
  GLfloat xmax = znear / 2.f;

  glViewport(application.viewport.lower_left_x, application.viewport.lower_left_y, application.viewport.width,
             application.viewport.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xmax, xmax, -xmax * aspect, xmax * aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, (znear - zfar) / 2.f);
}

/* program & OpenGL initialization */
static void component_init(entt::registry* registry) {
  const auto& lights = registry->view<Light>();
  lights.each([](const Light& light) {
    if (!light.position().has_packed()) {
      throw std::logic_error("Light position data must be packed.");
    }
    if (!light.ambient().has_packed()) {
      throw std::logic_error("Material color data must be packed.");
    }
    if (!light.diffuse().has_packed()) {
      throw std::logic_error("Material color data must be packed.");
    }
    if (!light.specular().has_packed()) {
      throw std::logic_error("Material color data must be packed.");
    }
    glLightfv(GL_LIGHT0 + light.light_num(), GL_POSITION,
              reinterpret_cast<const float*>(light.position().packed().bytes().data()));
    glLightiv(GL_LIGHT0 + light.light_num(), GL_AMBIENT,
              reinterpret_cast<const int32_t*>(light.ambient().packed().bytes().data()));
    glLightiv(GL_LIGHT0 + light.light_num(), GL_DIFFUSE,
              reinterpret_cast<const int32_t*>(light.diffuse().packed().bytes().data()));
    glLightiv(GL_LIGHT0 + light.light_num(), GL_SPECULAR,
              reinterpret_cast<const int32_t*>(light.specular().packed().bytes().data()));
    if (light.enabled()) {
      glEnable(GL_LIGHT0 + light.light_num());
    } else {
      glDisable(GL_LIGHT0 + light.light_num());
    }
  });
  glEnable(GL_LIGHTING);

  /*
  constexpr int NUM_LIGHTS{4};
  const GLfloat light_positions[][NUM_LIGHTS] = {
      {10.f, 0.f, 0.f, 0.f},
      {0.f, 10.f, 0.f, 0.f},
      {0.f, 0.f, 10.f, 0.f},
      {0.f, 0.f, 0.f, 10.f},
  };
  const GLfloat light_diffuse_color[][NUM_LIGHTS] = {
      {0.4f, 0.4f, 0.4f, 0.4f},
      {0.4f, 0.4f, 0.4f, 0.4f},
      {0.4f, 0.4f, 0.4f, 0.4f},
      {0.4f, 0.4f, 0.4f, 0.4f},
  };
  for (int i = 0; i < NUM_LIGHTS; ++i) {
  }
  */

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Draw the gears.
  auto gears = registry->view<Gear, Position, Orientation>();
  gears.each([registry](const entt::registry::entity_type& entity, const Gear& gear_parameters,
                        const Position& position, const Orientation& orientation) {
    GLId id{};
    id.gl_id = build_gear(gear_parameters);
    registry->emplace<GLId>(entity, id);
  });

  // Is this needed?
  // glEnable(GL_NORMALIZE);
}

const char* determine_glsl_version() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  const char* glsl_version = "#version 100";
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
  const char* glsl_version = "#version 150";
#else
  // GL 3.3 + GLSL 130
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
  const char* glsl_version = "#version 130";
#endif
  return glsl_version;
}

void gui_draw(const entt::registry& registry) {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  bool show_demo_window{false};
  ImGui::ShowDemoWindow(&show_demo_window);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace pack::gear

int main(int argc, char* argv[]) {
  using namespace pack::color;
  using namespace pack::gear;
  using namespace pack::lighting;
  using namespace pack::proto;

  // Default logging configuration.
  FLAGS_logtostderr = true;
  FLAGS_stderrthreshold = 0;
  google::InitGoogleLogging(argv[0]);

  // But allow user to override via the command line.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  GLFWwindow* window;
  int width = 1920;
  int height = 1080;

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &width, &height);
    window = glfwCreateWindow(width, height, "Gears", monitor, NULL);
    if (!window) {
      fprintf(stderr, "Failed to open GLFW window\n");
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
  }

  // Setup Dear ImGui context. Must be done before calling ImGui::GetIO().
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  Application app{ImGui::GetIO()};
  app.viewport = {0, 0, width, height};
  glfwSetWindowUserPointer(window, &app);

  // Set callback functions
  glfwSetFramebufferSizeCallback(window, reshape);
  glfwSetKeyCallback(window, key);

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);

  glfwGetFramebufferSize(window, &width, &height);
  reshape(window, width, height);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(determine_glsl_version());

  app.scene_parameters = app.registry.create();
  app.registry.emplace<SceneParameters>(app.scene_parameters, SceneParameters{});

  {
    LOG(INFO) << "Loading gears";
    Gears gears = load_text_proto<Gears>("gear/trivial_demo_gears.pb.txt");
    LOG(INFO) << "Gears loaded:\n" << gears.DebugString();

    for (Gear gear : gears.gear()) {
      Material* material = gear.mutable_material();
      Materials::pack_colors(material);
      const auto gear_id = app.registry.create();
      app.registry.emplace<Gear>(gear_id, std::move(gear));
      Position position{};
      position.mutable_float_values()->set_x(-3.1f);
      position.mutable_float_values()->set_y(4.2f);
      app.registry.emplace<Position>(gear_id, position);
      app.registry.emplace<Orientation>(gear_id, Orientation{});
    }
    LOG(INFO) << "Loaded gears";
  }

  {
    LOG(INFO) << "Loading lighting";
    LightingConfiguration lighting_configuration = load_text_proto<LightingConfiguration>("gear/lighting_configuration.pb.txt");
    LOG(INFO) << "Generating lighting debug string";
    LOG(INFO) << "Lighting configuration:\n" << lighting_configuration.DebugString();
    for (Light light : lighting_configuration.light()) {
      light = Lights::as_packed(light);
      const auto light_id = app.registry.create();
      app.registry.emplace<Light>(light_id, std::move(light));
    }
  }

  component_init(&app.registry);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Since the component_draw overwrites the full window, we have to draw the gui afterwards.
    // If we can limit the component drawing to a particular region, then we may be able to optimize GUI redraws only if
    // changed/dirty.
    component_draw(app.registry);
    gui_draw(app.registry);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Update animation
    animate(&app);
  }

  // Cleanup and shutdown.

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  exit(EXIT_SUCCESS);
}
