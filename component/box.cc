#include "component/box.h"

#include "glog/logging.h"
#include "material/material.h"
#include "position/position.h"
#include "third_party/glfw/glfw.h"

namespace pack::component {

struct Box final {
  float width{};
  float height{};
  float depth{};

  material::Material material{};
};

GLint build_box(const Box& box) {
  const GLint id = glGenLists(1);
  glNewList(id, GL_COMPILE);

  glMaterialfv(GL_FRONT, GL_AMBIENT, box.material.ambient.values);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, box.material.diffuse.values);
  glMaterialfv(GL_FRONT, GL_SPECULAR, box.material.specular.values);
  glMaterialf(GL_FRONT, GL_SHININESS, box.material.shininess);

  glShadeModel(GL_FLAT);

  glNormal3f(0.f, 0.f, 1.f);

  // We model a rectangular box as two triangle fans. The first fan starts at (0,0,0) and traverses the triangles in the
  // "bottom left" of the box. The second fan starts at (width, height, depth) and traverses the "upper right" of the
  // box.
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(box.width, 0.f, 0.f);
  glVertex3f(0.f, box.height, 0.f);
  glVertex3f(0.f, 0.f, box.depth);
  glVertex3f(box.width, 0.f, 0.f);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(box.width, box.height, box.depth);
  glVertex3f(0.f, box.height, box.depth);
  glVertex3f(box.width, 0.f, box.depth);
  glVertex3f(box.width, box.height, 0.f);
  glVertex3f(0.f, box.height, box.depth);
  glEnd();

  glEndList();

  return id;
}

GLint build_box(const ParameterBinding::Set& bindings, const material::Material& material) {
  Box box{};
  box.width = as_float(as_literal(bindings.find("width")->value));
  box.height = as_float(as_literal(bindings.find("height")->value));
  box.depth = as_float(as_literal(bindings.find("depth")->value));
  box.material = material;
  return build_box(box);
}

}  // namespace pack::component
