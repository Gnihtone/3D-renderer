#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct Vector {
 private:
  glm::vec2 v;
};

struct Triangle2D {
  glm::vec2 points[3];
  Color color;
};
