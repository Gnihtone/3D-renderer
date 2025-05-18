#include "application.h"
#include "except.h"

int main() {
  try {
    auto app = renderer::Application();
    app.Run();
  } catch (...) {
    except::react();
  }
  return 0;
}
