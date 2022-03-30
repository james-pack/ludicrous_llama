#include "ui/gear_edit_pane.h"

#include "entt/entity/registry.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "position/position.h"
#include "ui/application.h"
#include "ui/model/animate.h"
#include "ui/model/gear.h"
#include "ui/render.h"

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
    auto models = registry.view<Gear, position::Position, position::Orientation>();
    models.each([&registry](const auto entity, Gear& gear, position::Position& position,
                            position::Orientation& orientation) {
      bool position_was_changed{false};
      bool component_was_changed{false};
      ImGui::PushID(gear.id.c_str());
      if (ImGui::TreeNodeEx(gear.id.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet)) {
        position_was_changed =
            ImGui::SliderFloat3("Position", position.position, -50.f, 50.f, "%.2f") || position_was_changed;
        component_was_changed =
            ImGui::SliderFloat("Inner radius", &gear.inner_radius, 0.f, 25.f, "%.2f") || component_was_changed;
        component_was_changed =
            ImGui::SliderFloat("Outer radius", &gear.outer_radius, 0.f, 25.f, "%.2f") || component_was_changed;
        component_was_changed = ImGui::SliderFloat("Width", &gear.width, 0.f, 25.f, "%.2f") || component_was_changed;
        component_was_changed = ImGui::SliderInt("Teeth", &gear.teeth, 2, 1000) || component_was_changed;
        component_was_changed =
            ImGui::SliderFloat("Tooth depth", &gear.tooth_depth, 0.f, 25.f, "%.2f") || component_was_changed;
        component_was_changed =
            ImGui::SliderFloat("Angular speed", &gear.angle_coefficient, 0.f, 1000.f, "%.2f") || component_was_changed;
        component_was_changed =
            ImGui::SliderFloat("Angular phase", &gear.phase, 0.f, 1000.f, "%.2f") || component_was_changed;
        ImGui::TreePop();
      }
      ImGui::PopID();
      if (component_was_changed) {
        // TODO(james): Clean up unused draw list. Perhaps by making the Render element manage creation and
        // destruction of these draw lists?
        int draw_list_id = build_gear(gear);
        registry.replace<Render>(entity, construct_draw_list_renderer(draw_list_id));
        registry.replace<Gear>(entity, gear);
      }
      if (position_was_changed) {
        registry.replace<position::Position>(entity, position);
      }
    });
  }

  ImGui::End();
}

}  // namespace pack::ui
