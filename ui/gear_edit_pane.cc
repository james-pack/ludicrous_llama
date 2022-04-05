#include "ui/gear_edit_pane.h"

#include "component/component.h"
#include "component/parameter.h"
#include "component/value.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "position/position.h"
#include "ui/application.h"

namespace pack::ui {

bool render_parameter(component::ParameterBinding& binding) {
  component::Value& value = component::as_literal(binding.value);
  if (component::get_type(value) == component::Type::FLOAT) {
    return ImGui::SliderFloat(binding.name.c_str(), &component::as_float(value), 0.f, 25.f, "%.2f");
  } else {
    return false;
  }
}

void GearEditPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  lower_left_x_ = lower_left_x;
  lower_left_y_ = lower_left_y;
  width_ = width;
  height_ = height;
}

void GearEditPane::render() {
  ImGui::SetNextWindowPos(ImVec2(lower_left_x_, lower_left_y_), true);
  ImGui::SetNextWindowSize(ImVec2(width_, height_), true);

  bool is_open{true};
  // Early out if the window is collapsed, as an optimization.
  if (ImGui::Begin("Edit components", &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
    auto& reg{registry()};
    auto models = reg.view<component::Component>();
    models.each([&reg](auto entity, component::Component& component) {
      // bool position_was_changed{false};
      bool component_was_changed{false};
      // ImGui::PushID();
      if (ImGui::TreeNodeEx(&component, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet, "%s",
                            component.name.c_str())) {
        for (const auto& const_binding : component.bindings) {
          component::ParameterBinding& binding{const_cast<component::ParameterBinding&>(const_binding)};
          component_was_changed = render_parameter(binding) || component_was_changed;
        }
        // position_was_changed =
        //     ImGui::SliderFloat3("Position", position.position, -50.f, 50.f, "%.2f") || position_was_changed;
        ImGui::TreePop();
      }
      // ImGui::PopID();
      if (component_was_changed) {
        reg.replace<component::Component>(entity, component);
      }
      // if (position_was_changed) {
      //   reg.replace<position::Position>(entity, position);
      // }
    });
  }

  ImGui::End();
}

}  // namespace pack::ui
