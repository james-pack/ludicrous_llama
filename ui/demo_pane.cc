#include "ui/demo_pane.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace pack::ui {

void DemoPane::set_bounds(int lower_left_x, int lower_left_y, int width, int height) {
  // Ignore for now.
}

void DemoPane::render() {
  bool show_demo_window{false};
  ImGui::ShowDemoWindow(&show_demo_window);
}

}  // namespace pack::ui
