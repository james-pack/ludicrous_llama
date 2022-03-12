#include "component/gear.h"

// TODO(james): Extract to a separate file to manage math constants. Configure to use C++20 std::numbers::pi when
// available. Feature testing macro: __cpp_lib_math_constants.
#if defined(_MSC_VER)
// Make MS math.h define M_PI
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <stdexcept>

#include "color/material.pb.h"
#include "color/materials.h"
#include "component/gear.pb.h"
#include "glog/logging.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {

GLint build_gear(const Gear& gear) {
  DLOG(INFO) << "Construct draw list identifier";
  const unsigned int id = glGenLists(1);
  DLOG(INFO) << "Instantiate draw list";
  glNewList(id, GL_COMPILE);

  DLOG(INFO) << "Add materials to draw list";
  const color::Material& material = gear.material();
  if (!material.ambient().has_packed()) {
    throw std::logic_error("Material color data must be packed.");
  }
  if (!material.diffuse().has_packed()) {
    throw std::logic_error("Material color data must be packed.");
  }
  if (!material.specular().has_packed()) {
    throw std::logic_error("Material color data must be packed.");
  }
  glMaterialiv(GL_FRONT, GL_AMBIENT, reinterpret_cast<const int32_t*>(material.ambient().packed().bytes().data()));
  glMaterialiv(GL_FRONT, GL_DIFFUSE, reinterpret_cast<const int32_t*>(material.diffuse().packed().bytes().data()));
  glMaterialiv(GL_FRONT, GL_SPECULAR, reinterpret_cast<const int32_t*>(material.specular().packed().bytes().data()));
  glMaterialf(GL_FRONT, GL_SHININESS, material.shininess());

  DLOG(INFO) << "Add vertices to draw list";
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = gear.inner_radius();
  r1 = gear.outer_radius() - gear.tooth_depth() / 2.f;
  r2 = gear.outer_radius() + gear.tooth_depth() / 2.f;

  da = 2.f * (float)M_PI / gear.teeth() / 4.f;

  glShadeModel(GL_FLAT);

  glNormal3f(0.f, 0.f, 1.f);

  // TODO(james): Rework as triangles as quads are being deprecated.
  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width() * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width() * 0.5f);
    if (i < gear.teeth()) {
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width() * 0.5f);
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width() * 0.5f);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.f * (float)M_PI / gear.teeth() / 4.f;
  for (i = 0; i < gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();

    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), gear.width() * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width() * 0.5f);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width() * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width() * 0.5f);
    if (i < gear.teeth()) {
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width() * 0.5f);
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width() * 0.5f);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.f * (float)M_PI / gear.teeth() / 4.f;
  for (i = 0; i < gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();

    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -gear.width() * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width() * 0.5f);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();

    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width() * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width() * 0.5f);
    u = r2 * (float)cos(angle + da) - r1 * (float)cos(angle);
    v = r2 * (float)sin(angle + da) - r1 * (float)sin(angle);
    len = (float)sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -gear.width() * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), gear.width() * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -gear.width() * 0.5f);
    u = r1 * (float)cos(angle + 3 * da) - r2 * (float)cos(angle + 2 * da);
    v = r1 * (float)sin(angle + 3 * da) - r2 * (float)sin(angle + 2 * da);
    glNormal3f(v, -u, 0.f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width() * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width() * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
  }

  glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), gear.width() * 0.5f);
  glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), -gear.width() * 0.5f);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth(); i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth();
    glNormal3f(-(float)cos(angle), -(float)sin(angle), 0.f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width() * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width() * 0.5f);
  }
  glEnd();

  glEndList();

  return id;
}

Animate construct_gear_animator(const Gear& /* ignored */) {
  return [](double seconds, Gear& component, Position& /* ignored */, Orientation& orientation) {
    orientation.set_rot_z(2.f * 1000.f * seconds / component.teeth() * component.angle_coefficient() +
                          component.phase());
  };
}

}  // namespace pack::component
