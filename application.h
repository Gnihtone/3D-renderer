#pragma once

#include "components/graphics.h"
#include "components/runtime.h"
#include "components/visualizer.h"

namespace renderer {

class Application {
 public:
  Application();

  void Run();

 private:
  World world_;
  RunTime runtime_;
  Visualizer visualizer_;
  GraphicsPipeline graphics_pipeline_;
  Picture picture_;
};

}  // namespace renderer
