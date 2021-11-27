#include <docopt.h>

#include "App/App.h"

int main() {
  RTIAW::App app({500, 100}, {1280, 800});
  app.Run();

  return 0;
}
