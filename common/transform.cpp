#include "transform.h"

namespace renderer {

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
    : position_(position), rotation_(rotation), scale_(scale) {}

Transform::Transform(const Vector3& position, const Quaternion& rotation)
    : position_(position), rotation_(rotation), scale_(1) {}

Transform::Transform(const Vector3& position) : position_(position) {}

const Vector3& Transform::GetPosition() const {
  return position_;
}

void Transform::SetPosition(const Vector3& position) {
  position_ = position;
}

const Quaternion& Transform::GetRotation() const {
  return rotation_;
}

void Transform::SetRotation(const Quaternion& rotation) {
  rotation_ = rotation;
}

const Vector3& Transform::GetScale() const {
  return scale_;
}

void Transform::SetScale(const Vector3& scale) {
  scale_ = scale;
}

}  // namespace renderer
