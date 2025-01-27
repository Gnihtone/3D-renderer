#pragma once

#include <memory>
#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "geometry.h"

class Object {
 public:
  Object();
  ~Object() = default;

  Object(Object&&) = default;

  explicit Object(const std::vector<Triangle3D>& triangles);
  explicit Object(const std::vector<Triangle3D>& triangles, const glm::vec3& position);

  void AddChild(const std::shared_ptr<Object>& child);
  void RemoveChild(const std::shared_ptr<Object>& child);

  [[nodiscard]] const glm::vec3& GetPosition() const;
  void SetPosition(const glm::vec3& position);

  [[nodiscard]] const glm::vec3& GetScale() const;
  void SetScale(const glm::vec3& scale);

  [[nodiscard]] const glm::quat& GetRotation() const;
  void SetRotation(const glm::quat& rotation);

  [[nodiscard]] size_t GetTrianglesCountWithChildren() const;

  [[nodiscard]] std::vector<Triangle3D> GetTriangles() const;
  [[nodiscard]] std::vector<std::shared_ptr<Object>> GetChildren() const;

 private:
  glm::vec3 position_;
  glm::vec3 scale_;
  glm::quat rotation_;

  std::vector<Triangle3D> mesh_triangles_;
  std::vector<std::shared_ptr<Object>> children_;
};
