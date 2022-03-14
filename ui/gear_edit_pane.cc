#include "ui/gear_edit_pane.h"

#include "entt/entity/registry.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui/application.h"
#include "ui/model/animate.h"
#include "ui/model/gear.h"
#include "ui/model/position.h"

namespace pack::ui {

void GearEditPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  lower_left_x_ = lower_left_x;
  lower_left_y_ = lower_left_y;
  width_ = width;
  height_ = height;
}

void GearEditPane::render() {
  using namespace ui::model;

  ImGui::SetNextWindowPos(ImVec2(lower_left_x_, lower_left_y_), true);
  ImGui::SetNextWindowSize(ImVec2(width_, height_), true);

  bool is_open{true};
  // Early out if the window is collapsed, as an optimization.
  if (ImGui::Begin("Edit gears", &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
    entt::registry& registry{Application::current().registry()};
    auto models = registry.view<Gear, Position, Orientation>();
    models.each([&registry](const auto entity, Gear& gear, Position& position, Orientation& orientation) {
      bool position_was_changed{false};
      ImGui::PushID(gear.id.c_str());
      if (ImGui::TreeNodeEx(gear.id.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet)) {
        position_was_changed = ImGui::SliderFloat3("Position", position.position, -50.f, 50.f, "%.2f");
        ImGui::TreePop();
      }
      ImGui::PopID();
      if (position_was_changed) {
        registry.replace<Position>(entity, position);
      }
    });
  }

  ImGui::End();
}

}  // namespace pack::ui
