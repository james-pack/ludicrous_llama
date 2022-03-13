#include "ui/component_pane.h"

#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "third_party/glfw/glfw.h"
#include "ui/camera.h"
#include "ui/render.h"

namespace pack::ui {

void ComponentPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
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
}

void ComponentPane::render() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  const auto cameras = registry().view<const Camera, const component::Position, const component::Orientation>();
  cameras.each(
      [](const Camera& camera, const component::Position& position, const component::Orientation& orientation) {
        glRotatef(orientation.rot_x(), 1.0, 0.0, 0.0);
        glRotatef(orientation.rot_y(), 0.0, 1.0, 0.0);
        glRotatef(orientation.rot_z(), 0.0, 0.0, 1.0);
        glTranslatef(position.float_values().x(), position.float_values().y(), position.float_values().z());
      });

  const auto gears = registry().view<Render, component::Gear, component::Position, component::Orientation>();
  gears.each([](const Render& render, const component::Gear& component, const component::Position& position,
                const component::Orientation& orientation) {  //
    render(component, position, orientation);
  });

  glPopMatrix();
}

}  // namespace pack::ui
