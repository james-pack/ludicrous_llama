#include "ui/component_pane.h"

#include <string>

#include "component/component.h"
#include "entt/entity/observer.hpp"
#include "entt/entity/registry.hpp"
#include "glog/logging.h"
#include "lighting/light.h"
#include "position/position.h"
#include "render/camera.h"
#include "render/render_node.h"
#include "third_party/glfw/glfw.h"

namespace pack::ui {

void render_camera(const render::Camera& camera) {
  glRotatef(camera.orientation.orientation[0], 1.0, 0.0, 0.0);
  glRotatef(camera.orientation.orientation[1], 0.0, 1.0, 0.0);
  glRotatef(camera.orientation.orientation[2], 0.0, 0.0, 1.0);
  glTranslatef(camera.position.position[0], camera.position.position[1], camera.position.position[2]);
}

void render_light(const lighting::Light& light, const position::Position& position,
                  const position::Orientation& orientation) {
  if (light.enabled) {
    glEnable(GL_LIGHT0 + light.light_num);
    glLightfv(GL_LIGHT0 + light.light_num, GL_POSITION, position.position);
    glLightfv(GL_LIGHT0 + light.light_num, GL_AMBIENT, light.ambient.values);
    glLightfv(GL_LIGHT0 + light.light_num, GL_DIFFUSE, light.diffuse.values);
    glLightfv(GL_LIGHT0 + light.light_num, GL_SPECULAR, light.specular.values);
  } else {
    glDisable(GL_LIGHT0 + light.light_num);
  }
}

ComponentPane::ComponentPane()
    : camera_observer_(registry(),  //
                       entt::collector.group<render::Camera>().update<render::Camera>()),
      component_observer_(registry(),  //
                          entt::collector.group<component::Component, render::RenderNode>()
                              .update<component::Component>()
                              .update<render::RenderNode>()),
      lighting_observer_(registry(),  //
                         entt::collector.group<lighting::Light, position::Position, position::Orientation>()
                             .update<lighting::Light>()
                             .update<position::Position>()
                             .update<position::Orientation>()) {}

void ComponentPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  if ((lower_left_x != lower_left_x_) || (lower_left_y != lower_left_y_) || (width != width_) || (height != height_)) {
    lower_left_x_ = lower_left_x;
    lower_left_y_ = lower_left_y;
    width_ = width;
    height_ = height;

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
}

void ComponentPane::render() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const auto& reg{registry()};
  auto& comp_table{component_table()};

  if (requires_redraw_) {
    // DLOG(INFO) << "ComponentPane::render() -- full redraw was triggered.";
    glPushMatrix();

    reg.view<render::Camera>().each([](const auto entity, const render::Camera& camera) { render_camera(camera); });
    camera_observer_.clear();

    reg.view<render::RenderNode>().each(
        [&comp_table](const render::RenderNode& render_node) { render_node.render(comp_table); });
    component_observer_.clear();

    glPopMatrix();

    reg.view<lighting::Light, position::Position, position::Orientation>().each(
        [](const lighting::Light& light, const position::Position& position, const position::Orientation& orientation) {
          render_light(light, position, orientation);
        });
    lighting_observer_.clear();

    // requires_redraw_ = false;
  } else {
    DLOG(INFO) << "ComponentPane::render() -- Redrawing changes only.";
    glPushMatrix();

    camera_observer_.each([&reg](const auto entity) {
      const auto& camera = reg.get<render::Camera>(entity);
      render_camera(camera);
    });

    component_observer_.each([&reg, &comp_table](const auto entity) {
      const auto& render_node = reg.get<render::RenderNode>(entity);
      render_node.render(comp_table);
    });

    glPopMatrix();

    lighting_observer_.each([&reg](const auto entity) {
      const auto& [light, position, orientation] =
          reg.get<lighting::Light, position::Position, position::Orientation>(entity);
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
