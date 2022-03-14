#include "ui/light_edit_pane.h"

#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui/model/light.h"
#include "ui/model/position.h"

namespace pack::ui {

void LightEditPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  DLOG(INFO) << "LightEditPane::set_bounds() -- lower_left_x: " << lower_left_x << ", lower_left_y: " << lower_left_y
             << ", width: " << width << ", height: " << height;
  lower_left_x_ = lower_left_x;
  lower_left_y_ = lower_left_y;
  width_ = width;
  height_ = height;
}

void LightEditPane::render() {
  using namespace ui::model;

  ImGui::SetNextWindowPos(ImVec2(lower_left_x_, lower_left_y_), true);
  ImGui::SetNextWindowSize(ImVec2(width_, height_), true);

  bool is_open{false};
  // Early out if the window is collapsed, as an optimization.
  if (ImGui::Begin("Edit lighting", &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
    entt::registry& reg{registry()};
    auto models = reg.view<Light, Position, Orientation>();
    models.each([&reg](const auto entity, Light& light, Position& position, Orientation& orientation) {
      bool light_was_changed{false};
      bool position_was_changed{false};
      bool orientation_was_changed{false};
      ImGui::PushID(light.id.c_str());
      if (ImGui::TreeNodeEx(light.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet)) {
        position_was_changed =
            ImGui::SliderFloat3("Position", position.position, 0.f, 50.f, "%.2f") || position_was_changed;
        orientation_was_changed =
            ImGui::SliderFloat3("Orientation", orientation.orientation, 0.f, 50.f, "%.2f") || orientation_was_changed;
        light_was_changed = ImGui::ColorEdit4("Ambient", light.ambient.values) || light_was_changed;
        light_was_changed = ImGui::ColorEdit4("Diffuse", light.diffuse.values) || light_was_changed;
        light_was_changed = ImGui::ColorEdit4("Specular", light.specular.values) || light_was_changed;
        light_was_changed = ImGui::Checkbox("Enabled", &light.enabled) || light_was_changed;
        ImGui::TreePop();
      }
      ImGui::PopID();
      if (light_was_changed) {
        reg.replace<Light>(entity, light);
      }
      if (position_was_changed) {
        reg.replace<Position>(entity, position);
      }
      if (orientation_was_changed) {
        reg.replace<Orientation>(entity, orientation);
      }
    });
  }

  ImGui::End();
}

}  // namespace pack::ui
