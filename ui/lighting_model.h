#pragma once

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include "color/color.pb.h"
#include "color/colors.h"
#include "component/position.pb.h"
#include "component/positions.h"
#include "lighting/light.pb.h"
#include "lighting/lights.h"
#include "third_party/glfw/glfw.h"
#include "ui/signaller.h"

namespace pack::ui {

enum class LightingModelSignal : int8_t {
  INVALID,
  POSITION_UPDATE,
  COLOR_UPDATE,
  ENABLED_UPDATE,
  RESET_UPDATE,
  FILENAME_UPDATE,
};
std::string to_string(LightingModelSignal value);

class LightingModel final : public Signaller<LightingModel, LightingModelSignal> {
 private:
  struct LightModel final {
    GLint light_num;
    std::string name;
    GLfloat position[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat ambient[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat diffuse[4] = {0.f, 0.f, 0.f, 0.f};
    GLfloat specular[4] = {0.f, 0.f, 0.f, 0.f};
    GLboolean enabled{false};
  };

  std::vector<LightModel> lights_{};
  std::filesystem::path lighting_configuration_path_{};

  void reset_light(LightModel* light) {
    for (int i = 0; i < 4; ++i) {
      light->position[i] = 0.f;
      light->ambient[i] = 0.f;
      light->diffuse[i] = 0.f;
      light->specular[i] = 0.f;
    }
    light->enabled = false;
  }

  LightModel* find_light(GLint light_num) {
    for (auto& light : lights_) {
      if (light.light_num == light_num) {
        return &light;
      } else if (light.light_num > light_num) {
        break;
      }
    }
    return nullptr;
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
    using std::to_string;
    if (light_num < 0) {
      throw std::invalid_argument("Light numbers must be non-negative.");
    }
    if (light_num >= GL_MAX_LIGHTS) {
      throw std::invalid_argument("Light numbers must be less than GL_MAX_LIGHTS.");
    }

    auto iter = std::lower_bound(lights_.begin(), lights_.end(), light_num,
                                 [](const LightModel& entry, GLint light_num) { return entry.light_num < light_num; });
    if ((iter == lights_.end()) || (iter->light_num > light_num)) {
      return *lights_.insert(iter, LightModel{light_num, "Light " + to_string(light_num)});
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

  void reset();

  /**
   * Load the lighting configuration from the lighting_configuration_path.
   */
  void load();
  void load(const lighting::LightingConfiguration& lighting);

  /**
   * Save the lighting configuration to the lighting_configuration_path.
   */
  void save() const;
  void save(lighting::LightingConfiguration* lighting) const;

  const std::filesystem::path& lighting_configuration_path() const { return lighting_configuration_path_; }
  void set_lighting_configuration_path(const std::filesystem::path& lighting_configuration_path) {
    lighting_configuration_path_ = lighting_configuration_path;
  }

  GLboolean is_enabled(GLint light_num) const {
    const auto* light = find_light(light_num);
    if (light) {
      return light->enabled;
    } else {
      return false;
    }
  }

  GLboolean* enabled(GLint light_num) {
    auto* light = find_light(light_num);
    if (light) {
      return &light->enabled;
    } else {
      return nullptr;
    }
  }

  void enable(GLint light_num);
  void disable(GLint light_num);
  void erase(GLint light_num);

  const std::string* name(GLint light_num) const {
    const auto* light = find_light(light_num);
    if (light) {
      return &light->name;
    } else {
      return nullptr;
    }
  }

  void set_position(GLint light_num, GLfloat x, GLfloat y, GLfloat z, GLfloat w = 0.f);
  GLfloat* position(GLint light_num) {
    auto* light = find_light(light_num);
    if (light) {
      return light->position;
    } else {
      return nullptr;
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

  void set_ambient(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f);
  GLfloat* ambient(GLint light_num) {
    auto* light = find_light(light_num);
    if (light) {
      return light->ambient;
    } else {
      return nullptr;
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

  void set_diffuse(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f);
  GLfloat* diffuse(GLint light_num) {
    auto* light = find_light(light_num);
    if (light) {
      return light->diffuse;
    } else {
      return nullptr;
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

  void set_specular(GLint light_num, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.f);
  GLfloat* specular(GLint light_num) {
    auto* light = find_light(light_num);
    if (light) {
      return light->specular;
    } else {
      return nullptr;
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
};

}  // namespace pack::ui
