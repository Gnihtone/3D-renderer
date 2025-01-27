#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  static const Color RED;
  static const Color GREEN;
  static const Color BLUE;
  static const Color BLACK;
  static const Color WHITE;
  static const Color YELLOW;
  static const Color PURPLE;
};

struct Triangle3D {
  glm::vec3 points[3];
  Color color;
};

struct Triangle2D {
  glm::vec2 points[3];
  Color color;
};
