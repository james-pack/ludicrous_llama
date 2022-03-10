#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "color/color.pb.h"
#include "color/colors.h"
#include "component/position.pb.h"
#include "component/positions.h"
#include "lighting/light.pb.h"
#include "lighting/lights.h"
#include "ui/signaller.h"
#include "ui/ui.h"

namespace pack::ui {

enum class LightingModelSignal : int8_t {
  INVALID,
  POSITION_UPDATE,
  COLOR_UPDATE,
  ENABLED_UPDATE,
  RESET_UPDATE,
};

class LightingModel final : public Signaller<LightingModel, LightingModelSignal> {
 private:
  struct LightModel final {
    GLint light_num;
    GLfloat position[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat ambient[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat diffuse[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat specular[4] = {0.f, 0.f, 0.f, 0.f};
    GLboolean enabled{false};
  };

  std::vector<LightModel> lights_{};

  void reset_light(LightModel* light) {
    for (int i = 0; i < 4; ++i) {
      light->position[i] = 0.f;
      light->ambient[i] = 0.f;
      light->diffuse[i] = 0.f;
      light->specular[i] = 0.f;
    }
    light->enabled = false;
  }

  const LightModel* find_light(GLint light_num) const {
    for (const auto& light : lights_) {
      if (light.light_num == light_num) {
        return &light;
      } else if (light.light_num > light_num) {
        break;
      }
    }
    return nullptr;
  }

  void sort_lights() {
    std::sort(lights_.begin(), lights_.end(),
              [](const LightModel& lhs, const LightModel& rhs) { return lhs.light_num <= rhs.light_num; });
  }

  LightModel& create_light(GLint light_num) {
    auto iter = std::lower_bound(lights_.begin(), lights_.end(), light_num,
                                 [](const LightModel& entry, GLint light_num) { return entry.light_num < light_num; });
    if ((iter == lights_.end()) || (iter->light_num > light_num)) {
      return *lights_.insert(iter, LightModel{light_num});
    } else {
      using std::to_string;
      throw std::logic_error("Light with light_num of " + to_string(light_num) + " already exists.");
    }
  }

  LightModel& find_or_create_light(GLint light_num) {
    for (auto& light : lights_) {
      if (light.light_num == light_num) {
        return light;
      } else if (light.light_num > light_num) {
        break;
      }
    }
    return create_light(light_num);
  }

 public:
  LightingModel() = default;

  GLint max_light_num() const {
    if (!lights_.empty()) {
      return lights_.back().light_num;
    } else {
      return -1;
    }
  }

  void reset() {
    if (!lights_.empty()) {
      lights_.clear();
      signal(LightingModelSignal::RESET_UPDATE, *this);
    }
  }

  void configure(const lighting::LightingConfiguration& lighting) {
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

  void save(lighting::LightingConfiguration* lighting) const {
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

  void enable(GLint light_num) {
    auto& light = find_or_create_light(light_num);
    if (!light.enabled) {
      light.enabled = true;
      signal(LightingModelSignal::ENABLED_UPDATE, *this);
    }
  }

  void disable(GLint light_num) {
    auto& light = find_or_create_light(light_num);
    if (light.enabled) {
      light.enabled = false;
      signal(LightingModelSignal::ENABLED_UPDATE, *this);
    }
  }

  void erase(GLint light_num) {
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

  GLboolean is_enabled(GLint light_num) const {
    const auto* light = find_light(light_num);
    if (light) {
      return light->enabled;
    } else {
      return false;
    }
  }

  const GLfloat* position(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->position;
    } else {
      return nullptr;
    }
  }

  void set_position(GLint light_num, GLfloat x, GLfloat y, GLfloat z, GLfloat w = 0.f) {
    auto& light = find_or_create_light(light_num);
    const GLfloat values[4] = {x, y, z, w};
    if (std::memcmp(light.position, values, 4 * sizeof(GLfloat))) {
      std::memcpy(light.position, values, 4 * sizeof(GLfloat));
      signal(LightingModelSignal::POSITION_UPDATE, *this);
    }
  }

  GLfloat pos_x(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->position[0];
    } else {
      return 0.f;
    }
  }

  GLfloat pos_y(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->position[1];
    } else {
      return 0.f;
    }
  }

  GLfloat pos_z(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->position[2];
    } else {
      return 0.f;
    }
  }

  GLfloat pos_w(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->position[3];
    } else {
      return 0.f;
    }
  }

  const GLfloat* ambient(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient;
    } else {
      return nullptr;
    }
  }

  void set_ambient(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f) {
    auto& light = find_or_create_light(light_num);
    const GLfloat values[4] = {red, green, blue, alpha};
    if (std::memcmp(light.ambient, values, 4 * sizeof(GLfloat))) {
      std::memcpy(light.ambient, values, 4 * sizeof(GLfloat));
      signal(LightingModelSignal::COLOR_UPDATE, *this);
    }
  }

  const GLfloat* diffuse(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse;
    } else {
      return nullptr;
    }
  }

  void set_diffuse(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f) {
    auto& light = find_or_create_light(light_num);
    const GLfloat values[4] = {red, green, blue, alpha};
    if (std::memcmp(light.diffuse, values, 4 * sizeof(GLfloat))) {
      std::memcpy(light.diffuse, values, 4 * sizeof(GLfloat));
      signal(LightingModelSignal::COLOR_UPDATE, *this);
    }
  }

  const GLfloat* specular(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular;
    } else {
      return nullptr;
    }
  }

  void set_specular(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f) {
    auto& light = find_or_create_light(light_num);
    const GLfloat values[4] = {red, green, blue, alpha};
    if (std::memcmp(light.specular, values, 4 * sizeof(GLfloat))) {
      std::memcpy(light.specular, values, 4 * sizeof(GLfloat));
      signal(LightingModelSignal::COLOR_UPDATE, *this);
    }
  }

  GLfloat ambient_red(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient[0];
    } else {
      return 0.f;
    }
  }

  GLfloat ambient_green(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient[1];
    } else {
      return 0.f;
    }
  }

  GLfloat ambient_blue(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient[2];
    } else {
      return 0.f;
    }
  }

  GLfloat ambient_alpha(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient[3];
    } else {
      return 0.f;
    }
  }

  GLfloat diffuse_red(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse[0];
    } else {
      return 0.f;
    }
  }

  GLfloat diffuse_green(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse[1];
    } else {
      return 0.f;
    }
  }

  GLfloat diffuse_blue(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse[2];
    } else {
      return 0.f;
    }
  }

  GLfloat diffuse_alpha(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse[3];
    } else {
      return 0.f;
    }
  }

  GLfloat specular_red(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular[0];
    } else {
      return 0.f;
    }
  }

  GLfloat specular_green(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular[1];
    } else {
      return 0.f;
    }
  }

  GLfloat specular_blue(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular[2];
    } else {
      return 0.f;
    }
  }

  GLfloat specular_alpha(GLint light_num) const {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular[3];
    } else {
      return 0.f;
    }
  }
};

}  // namespace pack::ui
