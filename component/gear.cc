#include "component/gear.h"

// TODO(james): Extract to a separate file to manage math constants. Configure to use C++20 std::numbers::pi when
// available. Feature testing macro: __cpp_lib_math_constants.
#if defined(_MSC_VER)
// Make MS math.h define M_PI
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>

#include "glog/logging.h"
#include "material/material.h"
#include "position/position.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {
material::Material random_color() {
  material::Material result{};
  result.ambient.values[0] = std::numeric_limits<float>::max() / 2.f;
  result.ambient.values[1] = std::numeric_limits<float>::max() / 2.f;
  result.ambient.values[2] = std::numeric_limits<float>::max() / 2.f;
  result.ambient.values[3] = std::numeric_limits<float>::max() / 2.f;
  result.diffuse.values[0] = std::numeric_limits<float>::max() / 2.f;
  result.diffuse.values[1] = std::numeric_limits<float>::max() / 2.f;
  result.diffuse.values[2] = std::numeric_limits<float>::max() / 2.f;
  result.diffuse.values[3] = std::numeric_limits<float>::max() / 2.f;
  result.specular.values[0] = std::numeric_limits<float>::max() / 2.f;
  result.specular.values[1] = std::numeric_limits<float>::max() / 2.f;
  result.specular.values[2] = std::numeric_limits<float>::max() / 2.f;
  result.specular.values[3] = std::numeric_limits<float>::max() / 2.f;
  result.shininess = 128.f;
  return result;
}

struct Gear final {
  float inner_radius{};
  float outer_radius{};
  float width{};
  int32_t teeth{};
  float tooth_depth{};
  float angle_coefficient{};
  float phase{};

  material::Material material{random_color()};
};

GLint build_gear(const Gear& gear) {
  const GLint id = glGenLists(1);
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

GLint build_gear(const ParameterBinding::Set& bindings) {
  Gear gear{};
  DLOG(INFO) << "Building gear primitive. Bindings: " << to_string(bindings);
  gear.inner_radius = as_float(as_literal(bindings.find("inner_radius")->value));
  gear.outer_radius = as_float(as_literal(bindings.find("outer_radius")->value));
  gear.width = as_float(as_literal(bindings.find("width")->value));
  gear.teeth = as_integer(as_literal(bindings.find("teeth")->value));
  gear.tooth_depth = as_float(as_literal(bindings.find("tooth_depth")->value));
  gear.angle_coefficient = as_float(as_literal(bindings.find("angle_coefficient")->value));
  gear.phase = as_float(as_literal(bindings.find("phase")->value));
  return build_gear(gear);
}

}  // namespace pack::component
