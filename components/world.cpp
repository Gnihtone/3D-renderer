#include "world.h"

#include "../common/exceptions.h"

namespace renderer {
World::World(std::vector<Object>&& objects) : objects_(std::move(objects)) {}

Object& World::AddObject(Object&& object) {
  objects_.push_back(std::move(object));
  return objects_.back();
}

Object World::RemoveObject(const size_t object_id) {
  for (auto it = objects_.begin(); it != objects_.end(); ++it) {
    if (it->GetId() == object_id) {
      std::swap(*it, objects_.back());
      auto removed_object = std::move(objects_.back());
      objects_.pop_back();
      return removed_object;
    }
  }

  throw NotFoundException("Object in world not found");
}

std::vector<Object>& World::GetObjects() {
  return objects_;
}

const std::vector<Object>& World::GetObjects() const {
  return objects_;
}

const Object& World::GetObjectById(const ObjectId id) const {
  for (auto& object : objects_) {
    if (object.GetId() == id) {
      return object;
    }
  }

  throw NotFoundException("Object not found");
}

void World::AddCamera(Camera&& camera) {
  cameras_.push_back(std::move(camera));
}

void World::ChangeActiveCamera(size_t index) {
  if (cameras_.empty()) {
    throw NotFoundException("No cameras found");
  }

  if (index >= cameras_.size()) {
    index = cameras_.size() - 1;
  }
  active_camera_index = index;
}

const Camera& World::GetActiveCamera() const {
  return cameras_.at(active_camera_index);
}

Camera& World::GetActiveCamera() {
  return cameras_.at(active_camera_index);
}

void World::AddLight(SomeLight&& light) {
  lights_.push_back(std::move(light));
}

const std::vector<SomeLight>& World::GetLights() const {
  return lights_;
}

}  // namespace renderer
