#include "light.h"

namespace renderer {

Light::Light(Color color) : color_(std::move(color)) {}

const Color& Light::GetColor() const {
  return color_;
}

void Light::SetColor(Color color) {
  color_ = std::move(color);
}

Vector4 AmbientLight::CalculateColorComponent(const Triangle3D& triangle) const {
  return {color_.r / 255.f, color_.g / 255.f, color_.b / 255.f, color_.a / 255.f};
}

DirectionalLight::DirectionalLight(Color color, Vector3 direction)
    : Light(color), direction_(glm::normalize(std::move(direction))) {}

const Vector3& DirectionalLight::GetDirection() const {
  return direction_;
}

void DirectionalLight::SetDirection(Vector3 direction) {
  direction_ = glm::normalize(std::move(direction));
}

Vector4 DirectionalLight::CalculateColorComponent(const Triangle3D& triangle) const {
  const auto triangle_normal = glm::normalize(triangle.GetNormalVector());
  Vector4 component = {color_.r / 255.f, color_.g / 255.f, color_.b / 255.f, color_.a / 255.f};
  return component * glm::max(0.f, glm::dot(triangle_normal, direction_));
}

}  // namespace renderer
