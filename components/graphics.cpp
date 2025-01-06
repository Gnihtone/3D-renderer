#include "graphics.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/intersect.hpp>

namespace {

float GetMaxY(const Triangle3D& triangle) {
  float max_y = 0;
  for (int i = 0; i < 3; ++i) {
    max_y = std::max(triangle.points[i].y, max_y);
  }
  return max_y;
}

bool CheckPointIsInTriangle(const Triangle3D& triangle, const uint16_t x, const uint16_t y) {
  const auto sign = [](glm::vec3 point1, glm::vec3 point2) {
    return glm::cross(point1, point2).z > 0;
  };

  glm::vec3 p1(triangle.points[0]), p2(triangle.points[1]), p3(triangle.points[2]);
  glm::vec3 point(x + 0.5f, y + 0.5f, 0);

  const auto sign1 = sign(point - p1, p2 - p1);
  const auto sign2 = sign(point - p2, p3 - p2);
  const auto sign3 = sign(point - p3, p1 - p3);

  return (sign1 && sign2 && sign3) || !(sign1 || sign2 || sign3);
}

glm::vec2 FindTopMostPoint(const Triangle3D& triangle) {
  glm::vec2 point(UINT16_MAX, UINT16_MAX);
  for (int i = 0; i < 3; ++i) {
    if (triangle.points[i].y < point.y) {
      point = triangle.points[i];
    }
  }
  return point;
}

void TransformTriangles(std::vector<Triangle3D>& triangles, const glm::vec3& scale, const glm::vec3& position,
                        const glm::quat& rotation, const uint32_t width, const uint32_t height) {
  auto transform_matrix = glm::mat4(1.0f);
  const auto tmp = glm::mat4_cast(glm::normalize(rotation));
  transform_matrix = glm::translate(transform_matrix, position);
  transform_matrix = transform_matrix * tmp;
  transform_matrix = glm::scale(transform_matrix, scale);

  // const auto ortho_matrix = glm::ortho(0.f, 800.0f, .0f, 600.0f, 0.1f, 100.0f);
  // transform_matrix = ortho_matrix * transform_matrix;
  const auto perspective_matrix =
      glm::perspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.f);
  transform_matrix = perspective_matrix * transform_matrix;

  for (auto& [points, color] : triangles) {
    for (int i = 0; i < 3; ++i) {
      const auto new_point = transform_matrix * glm::vec4(points[i], 1);
      points[i] = new_point / new_point.w;
      points[i].x /= -1.f;
      points[i].z -= 1.f;
    }
  }
}

std::vector<Triangle3D> ProcessObject(const std::shared_ptr<Object>& object, const uint32_t width,
                                      const uint32_t height) {
  std::vector<Triangle3D> triangles;
  triangles.reserve(object->GetTrianglesCountWithChildren());
  for (const auto& child : object->GetChildren()) {
    auto child_triangles = ProcessObject(child, width, height);
    std::ranges::copy(child_triangles, std::back_inserter(triangles));
  }
  std::ranges::copy(object->GetTriangles(), std::back_inserter(triangles));

  TransformTriangles(triangles, object->GetScale(), object->GetPosition(), object->GetRotation(), width, height);
  return triangles;
}

bool IsInClip(const float coordinate) {
  return -1.f < coordinate && coordinate < 1.f;
}

Triangle3D ToViewPortCoordinates(const uint32_t width, const uint32_t height, const Triangle3D& triangle) {
  auto new_triangle = triangle;

  for (int i = 0; i < 3; ++i) {
    new_triangle.points[i].x += 1.f;
    new_triangle.points[i].x *= width / 2.f;
    new_triangle.points[i].y += 1.f;
    new_triangle.points[i].y *= height / 2.f;
  }
  return new_triangle;
}

float GetZCoordinate(const Triangle3D& triangle, const uint32_t x, const uint32_t y) {
  glm::vec3 intersection = {0, 0, -1};
  if (glm::intersectLineTriangle(glm::vec3(x + 0.5f, y + 0.5f, 0), glm::vec3(0, 0, 1), triangle.points[0],
                                 triangle.points[1], triangle.points[2], intersection)) {
    return intersection.x;
  }
  return -INFINITY;
}

std::vector<Color> TurnTrianglesIntoPixelsSlowWay(const uint32_t width, const uint32_t height,
                                                  const std::vector<Triangle3D>& triangles) {
  std::vector<Color> pixels(width * height, {3, 57, 108, 255});
  std::vector<float> z_buffer(width * height, -INFINITY);

  std::vector<Triangle3D> new_triangles;
  new_triangles.reserve(triangles.size());
  for (const auto& triangle : triangles) {
    new_triangles.push_back(ToViewPortCoordinates(width, height, triangle));
  }

  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      for (const auto& triangle : new_triangles) {
        auto z = GetZCoordinate(triangle, x, y);
        if (z == -INFINITY || z < z_buffer[y * width + x]) {
          continue;
        }
        z_buffer[y * width + x] = z;

        pixels[y * width + x] = triangle.color;
      }
    }
  }

  return pixels;
}

}  // namespace

std::vector<Color> TurnTrianglesIntoPixels(const uint32_t width, const uint32_t height,
                                           const std::vector<Triangle3D>& triangles) {
  std::vector<Color> pixels(width * height, {3, 57, 108, 255});
  std::vector<float> z_buffer(width * height, -INFINITY);

  for (const auto& triangle : triangles) {
    bool skip = false;
    for (int i = 0; i < 3; ++i) {
      if (!(IsInClip(triangle.points[i].x) && IsInClip(triangle.points[i].y)) && IsInClip(triangle.points[i].z)) {
        skip = true;
      }
    }
    if (skip) {
      continue;
    }

    const auto new_triangle = ToViewPortCoordinates(width, height, triangle);
    const auto top_most_point = FindTopMostPoint(new_triangle);

    const auto max_y = std::min(GetMaxY(new_triangle) + 1, static_cast<float>(height));

    for (int y = std::max(top_most_point.y, 0.f); y < max_y; ++y) {
      int32_t current_min_x = 0.f;
      int32_t current_max_x = width;
      // while (current_min_x > 0 && CheckPointIsInTriangle(new_triangle, current_min_x, y)) {
      //   --current_min_x;
      // }
      while (current_min_x < width - 1 && !CheckPointIsInTriangle(new_triangle, current_min_x, y)) {
        ++current_min_x;
      }
      // if (current_max_x < current_min_x) {
      //   current_max_x = current_min_x;
      // }

      // while (current_max_x < width - 1 && CheckPointIsInTriangle(new_triangle, current_max_x, y)) {
      //   ++current_max_x;
      // }
      while (current_max_x > current_min_x && !CheckPointIsInTriangle(new_triangle, current_max_x, y)) {
        --current_max_x;
      }
      // if (current_min_x > current_max_x) {
      //   current_min_x = current_max_x;
      // }

      for (int x = current_min_x; x <= current_max_x; ++x) {
        auto z = GetZCoordinate(new_triangle, x, y);
        auto vec_pos = y * width + x;
        if (z_buffer[vec_pos] < z) {
          pixels[vec_pos] = new_triangle.color;
          z_buffer[vec_pos] = z;
        }
      }
    }
  }

  return pixels;
}

std::vector<Color> GraphicsPipeline::Process(const World& world, const uint32_t width, const uint32_t height) const {
  // Vertex Shader
  size_t triangles_count = 0;
  for (const auto& object : world.GetObjects()) {
    triangles_count += object->GetTrianglesCountWithChildren();
  }

  std::vector<Triangle3D> triangles;
  triangles.reserve(triangles_count);
  for (const auto& object : world.GetObjects()) {
    std::ranges::copy(ProcessObject(object, width, height), std::back_inserter(triangles));
  }

  return TurnTrianglesIntoPixels(width, height, triangles);
}
