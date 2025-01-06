#include "world.h"

World::World(const std::vector<std::shared_ptr<Object>>& objects) : objects_(objects) {}

void World::AddObject(const std::shared_ptr<Object>& object) {
  objects_.push_back(object);
}

void World::RemoveObject(const std::shared_ptr<Object>& object) {
  for (auto it = objects_.begin(); it != objects_.end(); ++it) {
    if (*it == object) {
      std::swap(*it, objects_.back());
      objects_.pop_back();
    }
  }
}

std::vector<std::shared_ptr<Object>>& World::GetObjects() {
  return objects_;
}

const std::vector<std::shared_ptr<Object>>& World::GetObjects() const {
  return objects_;
}
