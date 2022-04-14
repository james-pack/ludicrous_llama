#pragma once

#include <string>

#include "component/component.h"
#include "render/camera.h"

namespace pack::render {

struct Scene {
  component::Subcomponent::Container root_components{};
  component::Component::Set components{};
  Camera::Container cameras{};

  bool operator==(const Scene& rhs) const {
    return root_components == rhs.root_components && components == rhs.components && cameras == rhs.cameras;
  }
};

std::string to_string(const Scene& scene);

}  // namespace pack::render
