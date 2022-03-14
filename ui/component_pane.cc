#include "ui/component_pane.h"

#include <string>

#include "entt/entity/observer.hpp"
#include "entt/entity/registry.hpp"
#include "glog/logging.h"
#include "third_party/glfw/glfw.h"
#include "ui/camera.h"
#include "ui/model/gear.h"
#include "ui/model/light.h"
#include "ui/model/position.h"
#include "ui/render.h"

namespace pack::ui {

void render_camera(const Camera& camera, const ui::model::Position& position,
                   const ui::model::Orientation& orientation) {
  using std::to_string;

  DLOG(INFO) << "Rendering camera -- orientation: " << to_string(orientation) << ", position: " << to_string(position);
  glRotatef(orientation.orientation[0], 1.0, 0.0, 0.0);
  glRotatef(orientation.orientation[1], 0.0, 1.0, 0.0);
  glRotatef(orientation.orientation[2], 0.0, 0.0, 1.0);
  glTranslatef(position.position[0], position.position[1], position.position[2]);
}

void render_light(const ui::model::Light& light, const ui::model::Position& position,
                  const ui::model::Orientation& orientation) {
  using std::to_string;

  if (light.enabled) {
    DLOG(INFO) << "Rendering light (enabled) -- light_num: " << light.light_num
               << ", orientation: " << to_string(orientation) << ", position: " << to_string(position)
               << ", ambient: " << to_string(light.ambient) << ", diffuse: " << to_string(light.diffuse)
               << ", specular: " << to_string(light.specular);
    glEnable(GL_LIGHT0 + light.light_num);
    glLightfv(GL_LIGHT0 + light.light_num, GL_POSITION, position.position);
    glLightfv(GL_LIGHT0 + light.light_num, GL_AMBIENT, light.ambient.values);
    glLightfv(GL_LIGHT0 + light.light_num, GL_DIFFUSE, light.diffuse.values);
    glLightfv(GL_LIGHT0 + light.light_num, GL_SPECULAR, light.specular.values);
  } else {
    DLOG(INFO) << "light_num: " << light.light_num << " is disabled";
    glDisable(GL_LIGHT0 + light.light_num);
  }
}

ComponentPane::ComponentPane()
    : camera_observer_(registry(),  //
                       entt::collector.group<Camera, ui::model::Position, ui::model::Orientation>()
                           .update<Camera>()
                           .update<ui::model::Position>()
                           .update<ui::model::Orientation>()),
      component_observer_(registry(),  //
                          entt::collector.group<Render, ui::model::Gear, ui::model::Position, ui::model::Orientation>()
                              .update<Render>()
                              .update<ui::model::Gear>()
                              .update<ui::model::Position>()
                              .update<ui::model::Orientation>()),
      lighting_observer_(registry(),  //
                         entt::collector.group<ui::model::Light, ui::model::Position, ui::model::Orientation>()
                             .update<ui::model::Light>()
                             .update<ui::model::Position>()
                             .update<ui::model::Orientation>()) {}

void ComponentPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  DLOG(INFO) << "ComponentPane::set_bounds() -- lower_left_x: " << lower_left_x << ", lower_left_y: " << lower_left_y
             << ", width: " << width << ", height: " << height;
  // TODO(james): Modify to manage a component pane(s) separately from UI panes.
  GLfloat aspect = (GLfloat)height / (GLfloat)width;
  GLfloat znear = 2.f;
  GLfloat zfar = 50.f;
  GLfloat xmax = znear / 2.f;

  glViewport(lower_left_x, lower_left_y, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xmax, xmax, -xmax * aspect, xmax * aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, (znear - zfar) / 2.f);

  requires_redraw_ = true;
}

void ComponentPane::render() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const entt::registry& reg{registry()};

  if (requires_redraw_) {
    glPushMatrix();

    reg.view<Camera, ui::model::Position, ui::model::Orientation>().each(
        [&reg](const auto entity, const Camera& camera, const ui::model::Position& position,
               const ui::model::Orientation& orientation) {  //
          render_camera(camera, position, orientation);
        });
    camera_observer_.clear();

    reg.view<Render, ui::model::Gear, ui::model::Position, ui::model::Orientation>().each(
        [&reg](const Render& render, const ui::model::Gear& component, const ui::model::Position& position,
               const ui::model::Orientation& orientation) {
          DLOG(INFO) << "Rendering component '" << component.name << "'";
          render(component, position, orientation);
        });
    component_observer_.clear();

    glPopMatrix();

    reg.view<ui::model::Light, ui::model::Position, ui::model::Orientation>().each(
        [&reg](const ui::model::Light& light, const ui::model::Position& position,
               const ui::model::Orientation& orientation) { render_light(light, position, orientation); });
    lighting_observer_.clear();
  } else {
    glPushMatrix();

    camera_observer_.each([&reg](const auto entity) {
      const auto& [camera, position, orientation] =
          reg.get<Camera, ui::model::Position, ui::model::Orientation>(entity);
      render_camera(camera, position, orientation);
    });

    component_observer_.each([&reg](const auto entity) {
      const auto& [render, component, position, orientation] =
          reg.get<Render, ui::model::Gear, ui::model::Position, ui::model::Orientation>(entity);
      DLOG(INFO) << "Rendering component '" << component.name << "'";
      render(component, position, orientation);
    });

    glPopMatrix();

    lighting_observer_.each([&reg](const auto entity) {
      const auto& [light, position, orientation] =
          reg.get<ui::model::Light, ui::model::Position, ui::model::Orientation>(entity);
      render_light(light, position, orientation);
    });
  }

  glEnable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

}  // namespace pack::ui
