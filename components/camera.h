#pragma once
#include "../common/transform.h"

namespace renderer {

class Camera {
 public:
  Camera();
  explicit Camera(const Transform& transform);

  [[nodiscard]] const Transform& GetTransform() const;
  Transform& GetTransform();

  [[nodiscard]] Matrix4x4 GetViewMatrix() const;

 private:
  Transform transform_;
};

}  // namespace renderer
