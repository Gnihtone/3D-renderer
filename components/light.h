#pragma once
#include <variant>

#include "../common/geometry.h"

namespace renderer {

enum class LightType {
  Ambient,
  Directional,
  Point,
};

class Light {
 public:
  explicit Light(Color color);

  [[nodiscard]] const Color& GetColor() const;
  void SetColor(Color color);

 protected:
  Color color_;
};

class AmbientLight : public Light {
 public:
  using Light::Light;

  Vector4 CalculateColorComponent(const Triangle3D& triangle) const;
};

class DirectionalLight : public Light {
 public:
  DirectionalLight(Color color, Vector3 direction);

  [[nodiscard]] const Vector3& GetDirection() const;
  void SetDirection(Vector3 direction);

  Vector4 CalculateColorComponent(const Triangle3D& triangle) const;

 private:
  Vector3 direction_;
};

using SomeLight = std::variant<AmbientLight, DirectionalLight>;

}  // namespace renderer
