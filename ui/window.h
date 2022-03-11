#pragma once

#include "ui/application.h"
#include "ui/ui.h"

namespace pack::ui {

class Window final : public Service {
 private:
  GLFWwindow* window_;
  int width_;
  int height_;

 public:
  Window();
  ~Window();

  void do_iteration();
  void change_size(int width, int height);
  void close();
};

}  // namespace pack::ui
