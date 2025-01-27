#pragma once

#include "../common/geometry.h"
#include "../common/object.h"
#include "world.h"

std::vector<Color> TurnTrianglesIntoPixels(uint32_t width, uint32_t height, const std::vector<Triangle2D>& triangles);

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  ~GraphicsPipeline() = default;

  [[nodiscard]] std::vector<Color> Process(const World& world, const uint32_t width, const uint32_t height) const;
};
