#include "ui/lighting_im_render.h"

#include <stdexcept>
#include <string>

#include "glog/logging.h"
#include "third_party/glfw/glfw.h"
#include "ui/lighting_model.h"

namespace pack::ui {

void lighting_im_render(const LightingModel& lighting) {
  const GLint max_light_num{lighting.max_light_num()};
  for (GLint light_num = 0; light_num <= max_light_num; ++light_num) {
    if (lighting.is_enabled(light_num)) {
      DLOG(INFO) << "light_num: " << light_num << " is enabled";
      glEnable(GL_LIGHT0 + light_num);
      glLightfv(GL_LIGHT0 + light_num, GL_POSITION, lighting.position(light_num));
      glLightfv(GL_LIGHT0 + light_num, GL_AMBIENT, lighting.ambient(light_num));
      glLightfv(GL_LIGHT0 + light_num, GL_DIFFUSE, lighting.diffuse(light_num));
      glLightfv(GL_LIGHT0 + light_num, GL_SPECULAR, lighting.specular(light_num));
    } else {
      DLOG(INFO) << "light_num: " << light_num << " is disabled";
      glDisable(GL_LIGHT0 + light_num);
    }
  }
  glEnable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void lighting_im_signal_render(LightingModelSignal signal, const LightingModel& lighting) {
  using std::to_string;
  switch (signal) {
    case LightingModelSignal::POSITION_UPDATE:
      [[fallthrough]];
    case LightingModelSignal::COLOR_UPDATE:
      [[fallthrough]];
    case LightingModelSignal::ENABLED_UPDATE:
      [[fallthrough]];
    case LightingModelSignal::RESET_UPDATE:
      lighting_im_render(lighting);
      break;
    default:
      DLOG(INFO) << "lighting_im_draw() throwing";
      throw std::domain_error("No implementation defined for LightingModelSignal value " + to_string(signal));
  }
}

}  // namespace pack::ui
