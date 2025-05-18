#pragma once

#include "../common/geometry.h"
#include "object.h"
#include "world.h"

namespace renderer {
class Picture;

class GraphicsPipeline {
 public:
  GraphicsPipeline() = default;
  ~GraphicsPipeline() = default;

  void Process(const World& world, Picture* picture) const;

 private:
  std::vector<float> z_buffer_;
};

}  // namespace renderer
