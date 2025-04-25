#pragma once

#include <memory>
#include <vector>

#include <glm/gtc/quaternion.hpp>

#include "../common/geometry.h"
#include "../common/transform.h"
#include "../common/types.h"

namespace renderer {

class Object {
 public:
  Object();
  virtual ~Object() = default;

  Object(const Object&) = delete;
  Object(Object&&) = default;

  explicit Object(const std::vector<Triangle3D>& triangles);
  explicit Object(const std::vector<Triangle3D>& triangles, const Transform& transform);

  Object& operator=(const Object&) = delete;
  Object& operator=(Object&&) = default;

  bool operator==(const Object& other) const;

  void AddChild(Object&& child);
  Object RemoveChild(size_t id);

  Transform& GetTransform();
  [[nodiscard]] const Transform& GetTransform() const;

  [[nodiscard]] size_t GetTrianglesCountWithChildren() const;

  [[nodiscard]] const std::vector<Triangle3D>& GetTriangles() const;
  [[nodiscard]] const std::vector<Object>& GetChildren() const;

  [[nodiscard]] ObjectId GetId() const;

  [[nodiscard]] Matrix4x4 GetTransformationMatrix() const;

  virtual void OnUpdate();

 private:
  ObjectId id_;
  Transform transform_;

  std::vector<Triangle3D> mesh_triangles_;
  std::vector<Object> children_;
};

}  // namespace renderer
