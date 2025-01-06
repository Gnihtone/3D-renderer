#pragma once

#include "../common/geometry.h"
#include "../common/object.h"

std::vector<Color> TurnTrianglesIntoPixels(uint32_t width, uint32_t height, const std::vector<Triangle2D>& triangles);

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  ~GraphicsPipeline() = default;

  std::vector<Color> Process(const std::vector<std::shared_ptr<Object>>& objects) const;
};
