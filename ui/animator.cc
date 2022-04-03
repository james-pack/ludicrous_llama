#include "ui/animator.h"

#include <chrono>

#include "entt/entity/registry.hpp"
#include "position/position.h"
#include "ui/model/animate.h"

namespace pack::ui {

void Animator::animate() {}

void Animator::do_iteration() { animate(); }

}  // namespace pack::ui
