#pragma once

#include <glm/fwd.hpp>

namespace renderer {

enum ObjectId : size_t;
enum Width : uint32_t;
enum Height : uint32_t;

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Quaternion = glm::quat;

using Matrix3x3 = glm::mat3;
using Matrix3x2 = glm::mat3x2;

using Matrix4x4 = glm::mat4;

}  // namespace renderer
