#include <docopt.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "App/App.h"

int main() {

  RTIAW::App app({100, 100}, {1280, 800});
  app.Run();

  return 0;
}
