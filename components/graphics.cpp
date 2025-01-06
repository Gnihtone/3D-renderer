#include "graphics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace {

float GetMaxY(const Triangle2D& triangle) {
  float max_y = 0;
  for (int i = 0; i < 3; ++i) {
    max_y = std::max(triangle.points[i].y, max_y);
  }
  return max_y;
}

bool CheckPointIsInTriangle(const Triangle2D& triangle, const uint16_t x, const uint16_t y) {
  const auto sign = [](glm::vec3 point1, glm::vec3 point2) {
    return glm::cross(point1, point2).z > 0;
  };

  glm::vec3 p1(triangle.points[0], 0), p2(triangle.points[1], 0), p3(triangle.points[2], 0);
  glm::vec3 point(x + 0.5f, y + 0.5f, 0);

  const auto sign1 = sign(point - p1, p2 - p1);
  const auto sign2 = sign(point - p2, p3 - p2);
  const auto sign3 = sign(point - p3, p1 - p3);

  return (sign1 && sign2 && sign3) || !(sign1 || sign2 || sign3);
}

glm::vec2 FindTopMostPoint(const Triangle2D& triangle) {
  glm::vec2 point(UINT16_MAX, UINT16_MAX);
  for (int i = 0; i < 3; ++i) {
    if (triangle.points[i].y < point.y) {
      point = triangle.points[i];
    }
  }
  return point;
}

void TransformTriangles(std::vector<Triangle3D>& triangles, const glm::vec3& scale, const glm::vec3& position, const glm::quat& rotation) {
  glm::mat4 transform_matrix = glm::scale(glm::mat4(1.0f), scale);
  transform_matrix = glm::mat4_cast(glm::normalize(rotation)) * transform_matrix;
  transform_matrix = glm::translate(transform_matrix, position);

  for (auto& [points, color] : triangles) {
    for (int i = 0; i < 3; ++i) {
      points[i] = transform_matrix * glm::vec4(points[i], 1);
    }
  }
}

std::vector<Triangle3D> ProcessObject(std::shared_ptr<Object> object) {
  std::vector<Triangle3D> triangles;
  triangles.reserve(object->GetTrianglesCountWithChildren());
  for (const auto& child : object->GetChildren()) {
    const auto child_triangles = ProcessObject(child);
    std::ranges::copy(child_triangles, std::back_inserter(triangles));
  }
  std::ranges::copy(object->GetTriangles(), std::back_inserter(triangles));

  TransformTriangles(triangles, object->GetScale(), object->GetPosition(), object->GetRotation());
  return triangles;
}

} // namespace

std::vector<Color> TurnTrianglesIntoPixels(uint32_t width, uint32_t height, const std::vector<Triangle2D>& triangles) {
    std::vector<Color> pixels(width * height, {0, 0, 0, 255});

    for (const auto& triangle : triangles) {
        const auto top_most_point = FindTopMostPoint(triangle);
        int current_min_x = std::max(0.f, std::min(top_most_point.x, width - 1.f));
        int current_max_x = current_min_x;

        const auto max_y = std::min(GetMaxY(triangle) + 1, static_cast<float>(height));

        for (int y = std::max(top_most_point.y, 0.f); y < max_y; ++y) {
            while (current_min_x > 0 && CheckPointIsInTriangle(triangle, current_min_x, y)) {
                --current_min_x;
            }
            while (current_min_x < width - 1 && !CheckPointIsInTriangle(triangle, current_min_x, y)) {
                ++current_min_x;
            }
            if (current_max_x < current_min_x) {
                current_max_x = current_min_x;
            }

            while (current_max_x < width - 1 && CheckPointIsInTriangle(triangle, current_max_x, y)) {
                ++current_max_x;
            }
            while (current_max_x > 0 && !CheckPointIsInTriangle(triangle, current_max_x, y)) {
                --current_max_x;
            }
            if (current_min_x > current_max_x) {
                current_min_x = current_max_x;
            }

            for (int x = current_min_x; x <= current_max_x; ++x) {
                pixels[y * width + x] = triangle.color;
            }
        }
    }

    return pixels;
}

std::vector<Color> GraphicsPipeline::Process(const std::vector<std::shared_ptr<Object>>& objects) const {
  // Vertex Shader
  size_t triangles_count = 0;
  for (const auto& object : objects) {
    triangles_count += object->GetTrianglesCountWithChildren();
  }

  std::vector<Triangle3D> triangles;
  triangles.reserve(triangles_count);
  for (const auto& object : objects) {
    std::ranges::copy(ProcessObject(object), std::back_inserter(triangles));
  }


}
