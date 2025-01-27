#pragma once

#include <memory>

#include "../common/object.h"

class World {
public:
  World() = default;
  ~World() = default;

  explicit World(const std::vector<std::shared_ptr<Object>>& objects);

  void AddObject(const std::shared_ptr<Object>& object);
  void RemoveObject(const std::shared_ptr<Object>& object);

  std::vector<std::shared_ptr<Object>>& GetObjects();
  [[nodiscard]] const std::vector<std::shared_ptr<Object>>& GetObjects() const;

private:
  std::vector<std::shared_ptr<Object>> objects_;
};
