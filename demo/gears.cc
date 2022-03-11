// clang-format off
#include "ui/ui.h"
// clang-format on

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "ui/animator.h"
#include "ui/application.h"
#include "ui/window.h"

int main(int argc, char* argv[]) {
  // Default logging configuration.
  FLAGS_logtostderr = true;
  FLAGS_stderrthreshold = 0;
  google::InitGoogleLogging(argv[0]);

  // But allow user to override via the command line.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  pack::ui::Application application{};
  pack::ui::Window window{};
  application.add_service(window);
  pack::ui::Animator animator{};
  application.add_service(animator);

  application.execute();
}
