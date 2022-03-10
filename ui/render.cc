#include "ui/render.h"

#include "component/gear.pb.h"
#include "component/position.pb.h"
#include "ui/ui.h"

namespace pack::ui {

Render construct_draw_list_renderable(GLint draw_list_id) {
  return [draw_list_id](const component::Gear& /*unused*/, const component::Position& position,
                        const component::Orientation& orientation) {
    glPushMatrix();
    glTranslatef(position.float_values().x(), position.float_values().y(), position.float_values().z());
    glRotatef(orientation.rot_x(), 1.0, 0.0, 0.0);
    glRotatef(orientation.rot_y(), 0.0, 1.0, 0.0);
    glRotatef(orientation.rot_z(), 0.0, 0.0, 1.0);
    glCallList(draw_list_id);
    glPopMatrix();
  };
}

}  // namespace pack::ui
