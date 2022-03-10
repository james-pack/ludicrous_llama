#include "ui/lighting_model.h"

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include "color/color.pb.h"
#include "color/colors.h"
#include "component/position.pb.h"
#include "component/positions.h"
#include "glog/logging.h"
#include "lighting/light.pb.h"
#include "lighting/lights.h"
#include "proto/proto_utils.h"
#include "ui/ui.h"

namespace pack::ui {

std::string to_string(LightingModelSignal value) {
  using std::to_string;
  switch (value) {
    case LightingModelSignal::INVALID:
      return "INVALID";
    case LightingModelSignal::POSITION_UPDATE:
      return "POSITION_UPDATE";
    case LightingModelSignal::COLOR_UPDATE:
      return "COLOR_UPDATE";
    case LightingModelSignal::ENABLED_UPDATE:
      return "ENABLED_UPDATE";
    case LightingModelSignal::RESET_UPDATE:
      return "RESET_UPDATE";
    case LightingModelSignal::FILENAME_UPDATE:
      return "FILENAME_UPDATE";
    default:
      typename std::underlying_type_t<LightingModelSignal> unknown_value{
          static_cast<std::underlying_type_t<LightingModelSignal>>(value)};
      throw std::domain_error("to_string not implemented for given LightingModelSignal enum value '" +
                              to_string(unknown_value) + "'");
  }
}

void LightingModel::reset() {
  if (!lights_.empty()) {
    lights_.clear();
    signal(LightingModelSignal::RESET_UPDATE, *this);
  }
}

void LightingModel::load() {
  const lighting::LightingConfiguration lighting_configuration =
      proto::load_text_proto<lighting::LightingConfiguration>(lighting_configuration_path_);
  load(lighting_configuration);
}

void LightingModel::save() const {
  lighting::LightingConfiguration lighting_configuration{};
  save(&lighting_configuration);
  proto::save_text_proto(lighting_configuration_path_, lighting_configuration);
}

void LightingModel::load(const lighting::LightingConfiguration& lighting) {
  DLOG(INFO) << "Lighting configuration:\n" << lighting.DebugString();
  bool dirty{false};

  if (!lights_.empty()) {
    lights_.clear();
    dirty = true;
  }

  for (const auto& config : lighting.light()) {
    LightModel& light = find_or_create_light(config.light_num());
    // It is possible for a configuration to provide multiple settings for the same light number. In this case, the
    // later configuration overrides the earlier one.
    reset_light(&light);
    light.light_num = config.light_num();
    const component::Position position = component::Positions::as_floats(config.position());
    light.position[0] = config.position().float_values().x();
    light.position[1] = config.position().float_values().y();
    light.position[2] = config.position().float_values().z();
    light.position[3] = config.position().float_values().w();
    color::Rgba color = color::Colors::as_floats(config.ambient());
    light.ambient[0] = color.float_values().red();
    light.ambient[1] = color.float_values().green();
    light.ambient[2] = color.float_values().blue();
    light.ambient[3] = color.float_values().alpha();
    color = color::Colors::as_floats(config.diffuse());
    light.diffuse[0] = color.float_values().red();
    light.diffuse[1] = color.float_values().green();
    light.diffuse[2] = color.float_values().blue();
    light.diffuse[3] = color.float_values().alpha();
    color = color::Colors::as_floats(config.specular());
    light.specular[0] = color.float_values().red();
    light.specular[1] = color.float_values().green();
    light.specular[2] = color.float_values().blue();
    light.specular[3] = color.float_values().alpha();
    light.enabled = config.enabled();
    dirty = true;
  }

  if (dirty) {
    signal(LightingModelSignal::RESET_UPDATE, *this);
  }
}

void LightingModel::save(lighting::LightingConfiguration* lighting) const {
  lighting->Clear();
  for (const auto& light : lights_) {
    auto* config = lighting->add_light();
    config->set_light_num(light.light_num);
    config->mutable_position()->mutable_float_values()->set_x(light.position[0]);
    config->mutable_position()->mutable_float_values()->set_y(light.position[1]);
    config->mutable_position()->mutable_float_values()->set_z(light.position[2]);
    config->mutable_position()->mutable_float_values()->set_w(light.position[3]);

    config->mutable_ambient()->mutable_float_values()->set_red(light.ambient[0]);
    config->mutable_ambient()->mutable_float_values()->set_green(light.ambient[1]);
    config->mutable_ambient()->mutable_float_values()->set_blue(light.ambient[2]);
    config->mutable_ambient()->mutable_float_values()->set_alpha(light.ambient[3]);

    config->mutable_diffuse()->mutable_float_values()->set_red(light.diffuse[0]);
    config->mutable_diffuse()->mutable_float_values()->set_green(light.diffuse[1]);
    config->mutable_diffuse()->mutable_float_values()->set_blue(light.diffuse[2]);
    config->mutable_diffuse()->mutable_float_values()->set_alpha(light.diffuse[3]);

    config->mutable_specular()->mutable_float_values()->set_red(light.specular[0]);
    config->mutable_specular()->mutable_float_values()->set_green(light.specular[1]);
    config->mutable_specular()->mutable_float_values()->set_blue(light.specular[2]);
    config->mutable_specular()->mutable_float_values()->set_alpha(light.specular[3]);

    config->set_enabled(light.enabled);
  }
}

void LightingModel::enable(GLint light_num) {
  auto& light = find_or_create_light(light_num);
  if (!light.enabled) {
    light.enabled = true;
    signal(LightingModelSignal::ENABLED_UPDATE, *this);
  }
}

void LightingModel::disable(GLint light_num) {
  auto& light = find_or_create_light(light_num);
  if (light.enabled) {
    light.enabled = false;
    signal(LightingModelSignal::ENABLED_UPDATE, *this);
  }
}

void LightingModel::erase(GLint light_num) {
  auto iter = std::lower_bound(lights_.begin(), lights_.end(), light_num,
                               [](const LightModel& entry, GLint light_num) { return entry.light_num < light_num; });

  if (iter != lights_.end()) {
    const LightModel& light = *iter;
    if (light.enabled) {
      // By erasing this light, we are effectively disabling it.
      signal(LightingModelSignal::ENABLED_UPDATE, *this);
    }
    lights_.erase(iter);
  }
}

void LightingModel::set_position(GLint light_num, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  auto& light = find_or_create_light(light_num);
  const GLfloat values[4] = {x, y, z, w};
  if (std::memcmp(light.position, values, 4 * sizeof(GLfloat))) {
    std::memcpy(light.position, values, 4 * sizeof(GLfloat));
    signal(LightingModelSignal::POSITION_UPDATE, *this);
  }
}

void LightingModel::set_ambient(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  auto& light = find_or_create_light(light_num);
  const GLfloat values[4] = {red, green, blue, alpha};
  if (std::memcmp(light.ambient, values, 4 * sizeof(GLfloat))) {
    std::memcpy(light.ambient, values, 4 * sizeof(GLfloat));
    signal(LightingModelSignal::COLOR_UPDATE, *this);
  }
}

void LightingModel::set_diffuse(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  auto& light = find_or_create_light(light_num);
  const GLfloat values[4] = {red, green, blue, alpha};
  if (std::memcmp(light.diffuse, values, 4 * sizeof(GLfloat))) {
    std::memcpy(light.diffuse, values, 4 * sizeof(GLfloat));
    signal(LightingModelSignal::COLOR_UPDATE, *this);
  }
}

void LightingModel::set_specular(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  auto& light = find_or_create_light(light_num);
  const GLfloat values[4] = {red, green, blue, alpha};
  if (std::memcmp(light.specular, values, 4 * sizeof(GLfloat))) {
    std::memcpy(light.specular, values, 4 * sizeof(GLfloat));
    signal(LightingModelSignal::COLOR_UPDATE, *this);
  }
}

}  // namespace pack::ui
