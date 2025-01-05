#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Object {
 public:
  Object();
  ~Object();

 private:
  glm::vec3 position;
  glm::vec4 rotation;

  std::vector<Object> children;
};
