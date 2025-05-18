#include "camera.h"

#include <glm/gtc/quaternion.hpp>

namespace renderer {

Camera::Camera() : transform_() {}

Camera::Camera(const Transform& transform) : transform_(transform) {}

const Transform& Camera::GetTransform() const {
  return transform_;
}

Transform& Camera::GetTransform() {
  return transform_;
}

Matrix4x4 Camera::GetViewMatrix() const {
  auto translate = Matrix4x4(1.0f);
  translate = glm::translate(translate, -transform_.GetPosition());
  const auto rotate = glm::mat4_cast(glm::normalize(-transform_.GetRotation()));

  return rotate * translate;
}

}  // namespace renderer
