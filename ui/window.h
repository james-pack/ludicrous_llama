#pragma once

#include <vector>

#include "third_party/glfw/glfw.h"
#include "ui/application.h"
#include "ui/compass_layout.h"
#include "ui/pane.h"

namespace pack::ui {

class Window final : public Service {
 private:
  GLFWwindow* window_;
  int width_;
  int height_;

  Pane* pane_{nullptr};

 public:
  Window();
  ~Window();

  void do_iteration() override;
  void render();
  void change_size(int width, int height);
  void close();

  void set_pane(Pane& pane) { pane_ = &pane; }
};

}  // namespace pack::ui
