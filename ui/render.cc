#include "ui/render.h"

#include "glog/logging.h"
#include "position/position.h"
#include "third_party/glfw/glfw.h"
#include "ui/model/gear.h"

namespace pack::ui {

Render construct_draw_list_renderer(GLint draw_list_id) {
  return [draw_list_id](const ui::model::Gear& /*unused*/, const position::Position& position,
                        const position::Orientation& orientation) {
    glPushMatrix();
    glTranslatef(position.position[0], position.position[1], position.position[2]);
    glRotatef(orientation.orientation[0], 1.0, 0.0, 0.0);
    glRotatef(orientation.orientation[1], 0.0, 1.0, 0.0);
    glRotatef(orientation.orientation[2], 0.0, 0.0, 1.0);
    glCallList(draw_list_id);
    glPopMatrix();
  };
}

}  // namespace pack::ui
