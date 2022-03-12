#include "ui/imgui_framer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace pack::ui {

ImGuiFramer::ImGuiFramer() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

ImGuiFramer::~ImGuiFramer() {
  if (should_close_frame_) {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
}

ImGuiFramer::ImGuiFramer(ImGuiFramer&& rhs) : should_close_frame_(rhs.should_close_frame_) {
  rhs.should_close_frame_ = false;
}

ImGuiFramer& ImGuiFramer::operator=(ImGuiFramer&& rhs) {
  should_close_frame_ = rhs.should_close_frame_;
  rhs.should_close_frame_ = false;
  return *this;
}

}  // namespace pack::ui
