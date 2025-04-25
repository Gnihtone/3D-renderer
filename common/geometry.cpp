#include "geometry.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/vec3.hpp>

namespace renderer {

const Color Color::kRed = {255, 0, 0, 255};
const Color Color::kGreen = {0, 255, 0, 255};
const Color Color::kBlue = {0, 0, 255, 255};
const Color Color::kBlack = {0, 0, 0, 255};
const Color Color::kWhite = {255, 255, 255, 255};
const Color Color::kYellow = {255, 255, 0, 255};
const Color Color::kPurple = {255, 0, 255, 255};

Color Color::operator*(const Vector4& component) const {
  float _r = component.r;
  float _g = component.g;
  float _b = component.b;
  float _a = component.a;

  float r_ = static_cast<float>(r);
  float g_ = static_cast<float>(g);
  float b_ = static_cast<float>(b);
  float a_ = static_cast<float>(a);

  float r1 = _r * r_;
  float g1 = _g * g_;
  float b1 = _b * b_;
  float a1 = _a * a_;

  if (r1 > 255.f) {
    r1 = 255.f;
  }
  if (g1 > 255.f) {
    g1 = 255;
  }
  if (b1 > 255.f) {
    b1 = 255;
  }
  if (a1 > 255.f) {
    a1 = 255;
  }

  return Color(r1, g1, b1, a1);
}

Color Color::operator*(const float scalar) const {
  return Color(r * scalar, g * scalar, b * scalar, a * scalar);
}

Color Color::operator+(const Color& color) const {
  uint8_t _r = r + color.r;
  if (_r < r || _r < color.r) {
    _r = 255;
  }
  uint8_t _g = g + color.g;
  if (_g < g || _g < color.g) {
    _g = 255;
  }
  uint8_t _b = b + color.b;
  if (_b < b || _b < color.b) {
    _b = 255;
  }
  uint8_t _a = a + color.a;
  if (_a < a || _a < color.a) {
    _a = 255;
  }

  return Color(_r, _g, _b, _a);
}

Vector3 Triangle3D::GetNormalVector() const {
  const auto vec1 = points[2] - points[0];
  const auto vec2 = points[1] - points[0];

  return glm::normalize(glm::cross(vec1, vec2));
}

bool Triangle3D::IsClockwise() const {
  return GetNormalVector().z < 0.f;
}

}  // namespace renderer
