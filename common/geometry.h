#pragma once

#include <vector>

#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "types.h"

namespace renderer {

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  bool operator==(const Color& color) const = default;

  Color operator*(const Vector4& color) const;
  Color operator*(const float scalar) const;

  Color operator+(const Color& color) const;

  static const Color kRed;
  static const Color kGreen;
  static const Color kBlue;
  static const Color kBlack;
  static const Color kWhite;
  static const Color kYellow;
  static const Color kPurple;
};

struct Triangle3D {
  Matrix3x3 points;
  Color color;

  [[nodiscard]] Vector3 GetNormalVector() const;
  [[nodiscard]] bool IsClockwise() const;
};

struct Triangle2D {
  Matrix3x2 points;
  Color color;
};

}  // namespace renderer
