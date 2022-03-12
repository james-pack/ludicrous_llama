#include "ui/light_edit_pane.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui/application.h"
#include "ui/lighting_model.h"

namespace pack::ui {

void LightEditPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  // Ignore for now.
}

void LightEditPane::render() {
  entt::registry& registry{Application::current().registry()};
  auto models = registry.view<LightingModel>();
  models.each([](LightingModel& model) {
    for (int light_num = 0; light_num <= model.max_light_num(); ++light_num) {
      ImGui::PushID(model.name(light_num)->c_str());
      ImGui::Text("Light %d", light_num);
      ImGui::SliderFloat3("Position", static_cast<float*>(model.position(light_num)), 0.f, 50.f, "%.2f");
      ImGui::ColorEdit4("Ambient", static_cast<float*>(model.ambient(light_num)));
      ImGui::ColorEdit4("Diffuse", static_cast<float*>(model.diffuse(light_num)));
      ImGui::ColorEdit4("Specular", static_cast<float*>(model.specular(light_num)));
      ImGui::Checkbox("Enabled", reinterpret_cast<bool*>(model.enabled(light_num)));
      ImGui::PopID();
    }
    model.signal(LightingModelSignal::COLOR_UPDATE, model);
  });
}

}  // namespace pack::ui
