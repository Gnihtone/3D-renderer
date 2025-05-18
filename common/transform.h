#pragma once

#include "types.h"

#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>

namespace renderer {
class Transform {
 public:
  Transform() = default;

  explicit Transform(const Vector3& position);
  explicit Transform(const Vector3& position, const Quaternion& rotation);
  explicit Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

  [[nodiscard]] const Vector3& GetPosition() const;
  void SetPosition(const Vector3& position);

  [[nodiscard]] const Quaternion& GetRotation() const;
  void SetRotation(const Quaternion& rotation);

  [[nodiscard]] const Vector3& GetScale() const;
  void SetScale(const Vector3& scale);

 private:
  Vector3 position_ = Vector3{0};
  Quaternion rotation_ = Quaternion{1, 0, 0, 0};
  Vector3 scale_ = Vector3{1};
};

}  // namespace renderer
