#pragma once

#include <memory>

#include "camera.h"
#include "light.h"
#include "object.h"

namespace renderer {
class World {
 public:
  World() = default;
  explicit World(std::vector<Object>&& objects);

  Object& AddObject(Object&& object);
  Object RemoveObject(size_t object_id);

  std::vector<Object>& GetObjects();
  [[nodiscard]] const std::vector<Object>& GetObjects() const;

  [[nodiscard]] const Object& GetObjectById(const ObjectId id) const;

  void AddCamera(Camera&& camera);
  void ChangeActiveCamera(size_t index);

  [[nodiscard]] const Camera& GetActiveCamera() const;
  Camera& GetActiveCamera();

  void AddLight(SomeLight&& light);
  const std::vector<SomeLight>& GetLights() const;

 private:
  std::vector<Object> objects_;
  std::vector<Camera> cameras_;
  std::vector<SomeLight> lights_;

  size_t active_camera_index = 0;
};
}  // namespace renderer
