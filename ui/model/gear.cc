#include "ui/model/gear.h"

// TODO(james): Extract to a separate file to manage math constants. Configure to use C++20 std::numbers::pi when
// available. Feature testing macro: __cpp_lib_math_constants.
#if defined(_MSC_VER)
// Make MS math.h define M_PI
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>

#include "component/gear.pb.h"
#include "component/position.h"
#include "glog/logging.h"
#include "third_party/glfw/glfw.h"
#include "ui/model/animate.h"
#include "ui/model/material.h"

namespace pack::ui::model {

void Gear::from_proto(const component::Gear& proto, Gear* gear) {
  gear->id = proto.id();
  gear->name = proto.name();
  gear->inner_radius = proto.inner_radius();
  gear->outer_radius = proto.outer_radius();
  gear->width = proto.width();
  gear->teeth = proto.teeth();
  gear->tooth_depth = proto.tooth_depth();
  gear->angle_coefficient = proto.angle_coefficient();
  gear->phase = proto.phase();
  Material::from_proto(proto.material(), &gear->material);
}

Gear Gear::from_proto(const component::Gear& proto) {
  Gear result{};
  from_proto(proto, &result);
  return result;
}

void Gear::to_proto(const Gear& gear, component::Gear* proto) {
  proto->set_id(gear.id);
  proto->set_name(gear.name);
  proto->set_inner_radius(gear.inner_radius);
  proto->set_outer_radius(gear.outer_radius);
  proto->set_width(gear.width);
  proto->set_teeth(gear.teeth);
  proto->set_tooth_depth(gear.tooth_depth);
  proto->set_angle_coefficient(gear.angle_coefficient);
  proto->set_phase(gear.phase);
  Material::to_proto(gear.material, proto->mutable_material());
}

component::Gear Gear::to_proto(const Gear& gear) {
  component::Gear proto{};
  to_proto(gear, &proto);
  return proto;
}

Animate construct_gear_animator() {
  return [](double seconds, Gear& component, component::Position& /* ignored */, component::Orientation& orientation) {
    orientation.orientation[2] =
        2.f * 1000.f * seconds / component.teeth * component.angle_coefficient + component.phase;
  };
}

GLint build_gear(const Gear& gear) {
  const unsigned int id = glGenLists(1);
  glNewList(id, GL_COMPILE);

  glMaterialfv(GL_FRONT, GL_AMBIENT, gear.material.ambient.values);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, gear.material.diffuse.values);
  glMaterialfv(GL_FRONT, GL_SPECULAR, gear.material.specular.values);
  glMaterialf(GL_FRONT, GL_SHININESS, gear.material.shininess);

  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = gear.inner_radius;
  r1 = gear.outer_radius - gear.tooth_depth / 2.f;
  r2 = gear.outer_radius + gear.tooth_depth / 2.f;

  da = 2.f * (float)M_PI / gear.teeth / 4.f;

  glShadeModel(GL_FLAT);

  glNormal3f(0.f, 0.f, 1.f);

  // TODO(james): Rework as triangles as quads are being deprecated.
  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width * 0.5f);
    if (i < gear.teeth) {
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width * 0.5f);
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width * 0.5f);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.f * (float)M_PI / gear.teeth / 4.f;
  for (i = 0; i < gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;

    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), gear.width * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width * 0.5f);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width * 0.5f);
    if (i < gear.teeth) {
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width * 0.5f);
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width * 0.5f);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.f * (float)M_PI / gear.teeth / 4.f;
  for (i = 0; i < gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;

    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -gear.width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width * 0.5f);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;

    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), gear.width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -gear.width * 0.5f);
    u = r2 * (float)cos(angle + da) - r1 * (float)cos(angle);
    v = r2 * (float)sin(angle + da) - r1 * (float)sin(angle);
    len = (float)sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -gear.width * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), gear.width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -gear.width * 0.5f);
    u = r1 * (float)cos(angle + 3 * da) - r2 * (float)cos(angle + 2 * da);
    v = r1 * (float)sin(angle + 3 * da) - r2 * (float)sin(angle + 2 * da);
    glNormal3f(v, -u, 0.f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), gear.width * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -gear.width * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
  }

  glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), gear.width * 0.5f);
  glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), -gear.width * 0.5f);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= gear.teeth; i++) {
    angle = i * 2.f * (float)M_PI / gear.teeth;
    glNormal3f(-(float)cos(angle), -(float)sin(angle), 0.f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -gear.width * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), gear.width * 0.5f);
  }
  glEnd();

  glEndList();

  return id;
}

}  // namespace pack::ui::model
