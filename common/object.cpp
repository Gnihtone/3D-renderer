#include "object.h"

Object::Object() : position_(glm::vec3{0}), scale_(1), rotation_(1, 0, 0, 0), mesh_triangles_(), children_() {}

Object::Object(const std::vector<Triangle3D>& triangles) : Object(triangles, glm::vec3{0}) {}

Object::Object(const std::vector<Triangle3D>& triangles, const glm::vec3& position)
    : position_(position), scale_(1), rotation_(1, 0, 0, 0), mesh_triangles_(triangles), children_() {}

void Object::AddChild(const std::shared_ptr<Object>& child) {
  children_.push_back(child);
}

void Object::RemoveChild(const std::shared_ptr<Object>& child) {
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    if (*it == child) {
      std::swap(*it, children_.back());
      children_.pop_back();
      return;
    }
  }
}

std::vector<Triangle3D> Object::GetTriangles() const {
  return mesh_triangles_;
}

std::vector<std::shared_ptr<Object>> Object::GetChildren() const {
  return children_;
}

const glm::vec3& Object::GetPosition() const {
  return position_;
}

const glm::vec3& Object::GetScale() const {
  return scale_;
}

void Object::SetPosition(const glm::vec3& position) {
  position_ = position;
}

void Object::SetScale(const glm::vec3& scale) {
  scale_ = scale;
}

const glm::quat& Object::GetRotation() const {
  return rotation_;
}

void Object::SetRotation(const glm::quat& rotation) {
  rotation_ = rotation;
}

size_t Object::GetTrianglesCountWithChildren() const {
  size_t count = mesh_triangles_.size();
  for (const auto& child : children_) {
    count += child->GetTrianglesCountWithChildren();
  }
  return count;
}
