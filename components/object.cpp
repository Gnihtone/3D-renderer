#include "object.h"

#include "../common/exceptions.h"
#include "../generators/object_generator.h"

namespace renderer {

Object::Object() : id_(generators::GenerateObjectId()), transform_(), mesh_triangles_(), children_() {}

Object::Object(const std::vector<Triangle3D>& triangles) : Object(triangles, Transform()) {}

Object::Object(const std::vector<Triangle3D>& triangles, const Transform& transform)
    : id_(generators::GenerateObjectId()), transform_(transform), mesh_triangles_(triangles), children_() {}

bool Object::operator==(const Object& other) const {
  return id_ == other.id_;
}

void Object::AddChild(Object&& child) {
  children_.push_back(std::move(child));
}

Object Object::RemoveChild(const size_t id) {
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    if (it->GetId() == id) {
      std::swap(*it, children_.back());
      Object removed_object = std::move(children_.back());
      children_.pop_back();
      return removed_object;
    }
  }

  throw NotFoundException("Tried to remove a child that doesn't exist");
}

Transform& Object::GetTransform() {
  return transform_;
}

[[nodiscard]] const Transform& Object::GetTransform() const {
  return transform_;
}

size_t Object::GetTrianglesCountWithChildren() const {
  size_t count = mesh_triangles_.size();
  for (const auto& child : children_) {
    count += child.GetTrianglesCountWithChildren();
  }
  return count;
}

const std::vector<Triangle3D>& Object::GetTriangles() const {
  return mesh_triangles_;
}

const std::vector<Object>& Object::GetChildren() const {
  return children_;
}

ObjectId Object::GetId() const {
  return id_;
}

Matrix4x4 Object::GetTransformationMatrix() const {
  auto transform_matrix = Matrix4x4(1.0f);
  const auto tmp = glm::mat4_cast(glm::normalize(transform_.GetRotation()));
  transform_matrix = glm::translate(transform_matrix, transform_.GetPosition());
  transform_matrix = transform_matrix * tmp;
  transform_matrix = glm::scale(transform_matrix, transform_.GetScale());

  return transform_matrix;
}

void Object::OnUpdate() {}

}  // namespace renderer
