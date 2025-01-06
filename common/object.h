#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "geometry.h"

struct Object {
 public:
  Object() = default;
  ~Object() = default;

  Object(Object&&) = default;

  explicit Object(const std::vector<Triangle3D>& triangles);
  explicit Object(const std::vector<Triangle3D>& triangles, const glm::vec3& position);

  void AddChild(const std::shared_ptr<Object>& child);
  void RemoveChild(const std::shared_ptr<Object>& child);

  const glm::vec3& GetPosition() const;
  void SetPosition(const glm::vec3& position);

  const glm::vec3& GetScale() const;
  void SetScale(const glm::vec3& scale);

  const glm::quat& GetRotation() const;
  void SetRotation(const glm::quat& rotation);

  size_t GetTrianglesCountWithChildren() const;

  std::vector<Triangle3D> GetTriangles() const;
  std::vector<std::shared_ptr<Object>> GetChildren() const;

 private:
  glm::vec3 position_;
  glm::vec3 scale_;
  glm::quat rotation_;

  std::vector<Triangle3D> mesh_triangles_;
  std::vector<std::shared_ptr<Object>> children_;
};
