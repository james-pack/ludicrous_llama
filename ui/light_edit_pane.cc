#include "ui/light_edit_pane.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui/application.h"
#include "ui/lighting_model.h"

namespace pack::ui {

void LightEditPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  lower_left_x_ = lower_left_x;
  lower_left_y_ = lower_left_y;
  width_ = width;
  height_ = height;
}

void LightEditPane::render() {
  ImGui::SetNextWindowPos(ImVec2(lower_left_x_, lower_left_y_), true);
  ImGui::SetNextWindowSize(ImVec2(width_, height_), true);

  bool is_open{true};
  if (!ImGui::Begin("Edit lighting", &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
    // Early out if the window is collapsed, as an optimization.
    ImGui::End();
    return;
  }

  entt::registry& registry{Application::current().registry()};
  auto models = registry.view<LightingModel>();
  models.each([](const auto entity, LightingModel& model) {
    bool was_changed{false};
    for (int light_num = 0; light_num <= model.max_light_num(); ++light_num) {
      ImGui::PushID(model.name(light_num)->c_str());
      if (ImGui::TreeNodeEx(model.name(light_num)->c_str(),
                            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet)) {
        was_changed =
            ImGui::SliderFloat3("Position", static_cast<float*>(model.position(light_num)), 0.f, 50.f, "%.2f") ||
            was_changed;
        was_changed = ImGui::ColorEdit4("Ambient", static_cast<float*>(model.ambient(light_num))) || was_changed;
        was_changed = ImGui::ColorEdit4("Diffuse", static_cast<float*>(model.diffuse(light_num))) || was_changed;
        was_changed = ImGui::ColorEdit4("Specular", static_cast<float*>(model.specular(light_num))) || was_changed;
        was_changed = ImGui::Checkbox("Enabled", reinterpret_cast<bool*>(model.enabled(light_num))) || was_changed;
        ImGui::TreePop();
      }
      ImGui::PopID();
    }
    if (was_changed) {
      model.signal(LightingModelSignal::COLOR_UPDATE, model);
    }
  });
}

}  // namespace pack::ui
