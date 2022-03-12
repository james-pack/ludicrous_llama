#pragma once

namespace pack::ui {

/**
 * RAII class for initiliazing an ImGui frame at the beginning of drawing a GUI and calling render methods at the end.
 *
 * To use, instantiate once at the top of your render() call for your GUI. Ensure that the ImGuiFramer instance goes out
 * of scope before swapping out buffers in a double buffered system.
 *
 * Note that if you are using the Window class, this is already done for you.
 */
class ImGuiFramer final {
 private:
  // Is this instance responsible for closing the frame. Allows for move construction and assignment.
  bool should_close_frame_{true};

 public:
  ImGuiFramer();
  ImGuiFramer(ImGuiFramer&& rhs);
  ImGuiFramer& operator=(ImGuiFramer&& rhs);
  ~ImGuiFramer();
};

}  // namespace pack::ui
