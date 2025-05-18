#include "graphics.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>

#include "../common/picture.h"
#include "../common/types.h"

namespace renderer {
namespace {

bool ArefloatsEqual(const float a, const float b) {
  return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

float GetMaxY(const Triangle3D& triangle) {
  float max_y = 0;
  for (int i = 0; i < 3; ++i) {
    max_y = std::max(triangle.points[i].y, max_y);
  }
  return max_y;
}

bool IsPointInTriangle(const Triangle3D& triangle, const uint32_t x, const uint32_t y) {
  const auto sign = [](const Vector3& point1, const Vector3& point2) {
    return glm::cross(point1, point2).z > 0;
  };

  Vector3 p1(triangle.points[0]), p2(triangle.points[1]), p3(triangle.points[2]);
  Vector3 point(x + 0.5f, y + 0.5f, 0);

  const auto sign1 = sign(point - p1, p2 - p1);
  const auto sign2 = sign(point - p2, p3 - p2);
  const auto sign3 = sign(point - p3, p1 - p3);

  return (sign1 && sign2 && sign3) || !(sign1 || sign2 || sign3);
}

Vector3 FindHighestPoint(const Triangle3D& triangle) {
  Vector3 point(UINT16_MAX, UINT16_MAX, 0);
  for (int i = 0; i < 3; ++i) {
    if (triangle.points[i].y < point.y - std::numeric_limits<float>::epsilon() ||
        ArefloatsEqual(triangle.points[i].y, point.y) && triangle.points[i].x < point.x) {
      point = triangle.points[i];
    }
  }
  return point;
}

Vector3 FindLowestPoint(const Triangle3D& triangle) {
  Vector3 point(UINT16_MAX, 0, 0);
  for (int i = 0; i < 3; ++i) {
    if (triangle.points[i].y > point.y + std::numeric_limits<float>::epsilon() ||
        ArefloatsEqual(triangle.points[i].y, point.y) && triangle.points[i].x < point.x) {
      point = triangle.points[i];
    }
  }
  return point;
}

void TransformWorldTriangles(std::vector<Triangle3D>& triangles, const Object& object) {
  auto transform_matrix = object.GetModelMatrix();

  for (auto& [points, color] : triangles) {
    for (int i = 0; i < 3; ++i) {
      const auto new_point = transform_matrix * glm::vec4(points[i], 1);
      points[i] = new_point;
    }
  }
}

std::vector<Triangle3D> TransformWorldObjectTriangles(const Object& object) {
  std::vector<Triangle3D> triangles;
  triangles.reserve(object.GetTrianglesCountWithChildren());
  for (const auto& child : object.GetChildren()) {
    auto child_triangles = TransformWorldObjectTriangles(child);
    std::ranges::move(child_triangles, std::back_inserter(triangles));
  }
  std::ranges::copy(object.GetTriangles(), std::back_inserter(triangles));

  TransformWorldTriangles(triangles, object);
  return triangles;
}

void LightTriangles(std::vector<Triangle3D>& triangles, const World& world) {
  for (auto& triangle : triangles) {
    Vector4 component(0);
    for (const auto& light : world.GetLights()) {
      std::visit([&](const auto& light) { component += light.CalculateColorComponent(triangle); }, light);
    }
    triangle.color = triangle.color * component;
  }
}

void TransformViewTriangles(std::vector<Triangle3D>& triangles, const Camera& camera, const uint32_t width,
                            const uint32_t height) {
  auto transform_matrix = camera.GetViewMatrix();

  const auto perspective_matrix =
      glm::perspective<float>(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.f);
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

bool IsInClip(const float coordinate) {
  return -1.f < coordinate && coordinate < 1.f;
}

bool IsInClip(const Vector3& point) {
  return IsInClip(point.x) && IsInClip(point.y) && (point.z < 1.f && point.z > 0.f);
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
  Vector3 intersection = {0, 0, -1};
  if (glm::intersectLineTriangle(Vector3(x + 0.5f, y + 0.5f, 0), Vector3(0, 0, 1), triangle.points[0],
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

void IterateOverTriangle(Picture* picture, std::vector<float>* z_buffer, Vector2* first_point, Vector2* second_point,
                         const Vector3& base_vector1, const Vector3& base_vector2, const Vector3& from,
                         const Vector3& to, const Color& color, const Triangle3D& triangle) {
  const auto width = picture->GetWidth();
  for (int32_t y = floor(from.y); y < std::min(static_cast<Height>(ceil(to.y)), picture->GetHeight()); ++y) {
    first_point->x += base_vector1.x;
    first_point->y += base_vector1.z;
    second_point->x += base_vector2.x;
    second_point->y += base_vector2.z;

    Vector2 left_point, right_point;
    if (first_point->x < second_point->x) {
      left_point = *first_point;
      right_point = *second_point;
    } else {
      left_point = *second_point;
      right_point = *first_point;
    }

    Vector2 base_vector3 = right_point - left_point;
    base_vector3 /= base_vector3.x;
    float z = left_point.y;

    for (int32_t x = floor(left_point.x); x <= std::min(static_cast<Width>(ceil(right_point.x)), width); ++x) {
      if (!IsPointInTriangle(triangle, x, y)) {
        continue;
      }
      // z = GetZCoordinate(triangle, x, y);
      z += base_vector3.y;
      if (const auto vec_pos = y * width + x; z_buffer->at(vec_pos) < z) {
        picture->SetPixel(y, x, color);
        z_buffer->at(vec_pos) = z;
      }
    }
  }
}

void DrawLine(Picture* picture, std::vector<float>* z_buffer, Vector3 point_1, Vector3 point_2, const Color& color) {
  const auto width = picture->GetWidth();

  bool steep = false;
  if (std::abs(point_1.x - point_2.x) < std::abs(point_1.y - point_2.y)) {
    std::swap(point_1.x, point_1.y);
    std::swap(point_2.x, point_2.y);
    steep = true;
  }
  if (point_1.x > point_2.x) {
    std::swap(point_1, point_2);
  }
  int32_t dx = point_2.x - point_1.x;
  int32_t dy = point_2.y - point_1.y;
  float dz = point_2.z - point_1.z;
  int32_t derror2 = std::abs(dy) * 2;
  int32_t error2 = 0;
  int32_t y = point_1.y;
  for (int32_t x = point_1.x; x <= point_2.x; ++x) {
    const auto z = point_1.z + (static_cast<float>(x) - point_1.x) / (point_2.x - point_1.x) * dz;
    if (steep) {
      if (z_buffer->at(x * width + y) < z) {
        picture->SetPixel(x, y, color);
        z_buffer->at(x * width + y) = z;
      }
    } else {
      if (z_buffer->at(y * width + x) < z) {
        picture->SetPixel(y, x, color);
        z_buffer->at(y * width + x) = z;
      }
    }
    error2 += derror2;

    if (error2 > dx) {
      y += (point_2.y > point_1.y ? 1 : -1);
      error2 -= dx * 2;
    }
  }
}

void DrawOXLine(Picture* picture, std::vector<float>* z_buffer, Vector3 point_1, Vector3 point_2, const Color& color,
                const Triangle3D& triangle) {
  const auto width = picture->GetWidth();

  const auto y = static_cast<int32_t>(point_1.y);
  const auto x_init = roundf(point_1.x) + 0.5f;
  const auto x_end = roundf(point_2.x) - 0.5f;
  const auto z_init = GetZCoordinate(triangle, static_cast<int32_t>(x_init), y);
  const auto z_end = GetZCoordinate(triangle, static_cast<int32_t>(x_end), y);
  if (z_init == -INFINITY || z_end == -INFINITY) {
    return;
  }

  const auto dz = z_end - z_init;

  for (auto x = static_cast<int32_t>(x_init); x <= static_cast<int32_t>(x_end); ++x) {
    if (x >= width) {
      break;
    }

    auto dx = x - static_cast<int32_t>(x_init);

    const auto z = z_init + ((x_end - x_init) != 0 ? (static_cast<float>(dx) / (x_end - x_init)) * dz : 0);

    auto tmp_z = GetZCoordinate(triangle, x, y);
    bool ok = false;
    if (tmp_z != -INFINITY && std::abs(tmp_z - z) > 0.0001) {
      ok = true;
    }

    if (z < z_buffer->at(y * width + x) && color == Color::kWhite) {
      std::cout << "HEY";
    }

    if (z == -INFINITY || z < z_buffer->at(y * width + x)) {
      continue;
    }
    z_buffer->at(y * width + x) = z;

    picture->SetPixel(y, x, color);
  }
}

void DrawTriangle(Picture* picture, std::vector<float>* z_buffer, const Triangle3D& triangle) {
  const auto height = picture->GetHeight();

  auto highest_point = FindHighestPoint(triangle);
  auto lowest_point = FindLowestPoint(triangle);
  Vector3 middle_point;
  for (int i = 0; i < 3; ++i) {
    if (triangle.points[i] != highest_point && triangle.points[i] != lowest_point) {
      middle_point = triangle.points[i];
    }
  }

  highest_point.y = floor(highest_point.y);
  middle_point.y = round(middle_point.y);
  lowest_point.y = ceil(lowest_point.y);

  // DrawLine(picture, z_buffer, highest_point, middle_point, triangle.color);
  // DrawLine(picture, z_buffer, lowest_point, middle_point, triangle.color);
  // DrawLine(picture, z_buffer, lowest_point, highest_point, triangle.color);

  Vector3 base_vector_1 = lowest_point - highest_point;
  Vector3 base_vector_2 = middle_point - highest_point;
  Vector3 base_vector_3 = lowest_point - middle_point;

  const float height_1 = base_vector_1.y;
  const float height_2 = base_vector_2.y;
  const float height_3 = base_vector_3.y;

  for (int32_t y = 0; y < static_cast<int32_t>(height_2); ++y) {
    if (static_cast<float>(y) + highest_point.y >= static_cast<float>(height)) {
      break;
    }

    auto first_point = highest_point + base_vector_1 * (static_cast<float>(y) / height_1);
    auto second_point = highest_point + base_vector_2 * (static_cast<float>(y) / height_2);
    if (first_point.x > second_point.x) {
      std::swap(first_point.x, second_point.x);
    }

    DrawOXLine(picture, z_buffer, first_point, second_point, triangle.color, triangle);
  }
  for (auto y = static_cast<int32_t>(height_2); y < static_cast<int32_t>(height_1); ++y) {
    if (static_cast<float>(y) + highest_point.y >= static_cast<float>(height)) {
      break;
    }

    auto first_point = highest_point + base_vector_1 * (static_cast<float>(y) / height_1);
    auto second_point = middle_point + base_vector_3 * ((static_cast<float>(y) - height_2) / height_3);
    if (first_point.x > second_point.x) {
      std::swap(first_point.x, second_point.x);
    }

    DrawOXLine(picture, z_buffer, first_point, second_point, triangle.color, triangle);
  }
}

void TurnTrianglesIntoPixelsNewWay(Picture* picture, const std::vector<Triangle3D>& triangles) {
  const auto width = picture->GetWidth();
  const auto height = picture->GetHeight();
  picture->Clear({3, 57, 108, 255});
  std::vector<float> z_buffer(width * height, -INFINITY);

  for (const auto& triangle : triangles) {
    const auto new_triangle = ToViewPortCoordinates(width, height, triangle);

    DrawTriangle(picture, &z_buffer, new_triangle);

    // const auto highest_point = FindHighestPoint(new_triangle);
    // const auto lowest_point = FindLowestPoint(new_triangle);
    // Vector3 middle_point;
    // for (int i = 0; i < 3; ++i) {
    //   if (new_triangle.points[i] != highest_point && new_triangle.points[i] != lowest_point) {
    //     middle_point = new_triangle.points[i];
    //   }
    // }
    //
    // assert(highest_point != lowest_point);
    // assert(middle_point != lowest_point);
    // assert(middle_point != highest_point);
    // assert(highest_point.y <= middle_point.y);
    // assert(middle_point.y <= lowest_point.y);
    //
    // Vector3 base_vector_1 = lowest_point - highest_point;
    // Vector3 base_vector_2 = middle_point - highest_point;
    // Vector3 base_vector_3 = lowest_point - middle_point;
    //
    // base_vector_1 /= base_vector_1.y;
    // base_vector_2 /= base_vector_2.y;
    // base_vector_3 /= base_vector_3.y;
    //
    // Vector2 first_point{highest_point.x, highest_point.z};
    // Vector2 second_point{highest_point.x, highest_point.z};
    // IterateOverTriangle(picture, &z_buffer, &first_point, &second_point, base_vector_1, base_vector_2, highest_point,
    //                     middle_point, new_triangle.color, new_triangle);
    // second_point = Vector2{middle_point.x, middle_point.z};
    // IterateOverTriangle(picture, &z_buffer, &first_point, &second_point, base_vector_1, base_vector_3, middle_point,
    //                     lowest_point, new_triangle.color, new_triangle);
  }
}

void TurnTrianglesIntoPixels(Picture* picture, std::vector<Triangle3D>&& triangles, const World& world) {
  const auto width = picture->GetWidth();
  const auto height = picture->GetHeight();
  picture->Clear({3, 57, 108, 255});
  std::vector<float> z_buffer(width * height, -INFINITY);

  for (const auto& triangle : triangles) {
    if (!triangle.IsClockwise()) {
      continue;
    }

    size_t amount_of_points_in_clip = 0;
    for (size_t idx = 0; idx < 3; ++idx) {
      const auto& point = triangle.points[idx];
      if (IsInClip(point)) {
        ++amount_of_points_in_clip;
      }
    }
    if (amount_of_points_in_clip == 0) {
      continue;
    }

    const auto new_triangle = ToViewPortCoordinates(width, height, triangle);

    const auto highest_point = FindHighestPoint(new_triangle);
    const auto lowest_point = FindLowestPoint(new_triangle);

    const auto max_y = std::min(lowest_point.y + 1, static_cast<float>(height));

    int32_t current_min_x = 0;
    int32_t current_max_x = width - 1;

    for (int y = std::max(highest_point.y, 0.f); y < max_y; ++y) {
      current_min_x = std::max(0, current_min_x - 10);
      current_max_x = std::min(static_cast<int32_t>(width) - 1, current_max_x + 10);

      while (current_min_x > 0 && IsPointInTriangle(new_triangle, current_min_x, y)) {
        --current_min_x;
      }
      while (current_min_x < width - 1 && !IsPointInTriangle(new_triangle, current_min_x, y)) {
        ++current_min_x;
      }
      if (current_max_x < current_min_x) {
        current_max_x = current_min_x;
      }

      while (current_max_x < width - 1 && IsPointInTriangle(new_triangle, current_max_x, y)) {
        ++current_max_x;
      }
      while (current_max_x > 0 && !IsPointInTriangle(new_triangle, current_max_x, y)) {
        --current_max_x;
      }
      if (current_min_x > current_max_x) {
        current_min_x = current_max_x;
      }

      for (int x = current_min_x; x <= current_max_x; ++x) {
        auto z = GetZCoordinate(new_triangle, x, y);
        auto vec_pos = y * width + x;
        if (z_buffer[vec_pos] < z) {
          picture->SetPixel(y, x, new_triangle.color);
          z_buffer[vec_pos] = z;
        }
      }
    }
  }
}

}  // namespace

void GraphicsPipeline::Process(const World& world, Picture* picture) const {
  // Vertex Shader
  size_t triangles_count = 0;
  for (const auto& object : world.GetObjects()) {
    triangles_count += object.GetTrianglesCountWithChildren();
  }

  std::vector<Triangle3D> triangles;
  triangles.reserve(triangles_count);
  for (const auto& object : world.GetObjects()) {
    std::ranges::copy(TransformWorldObjectTriangles(object), std::back_inserter(triangles));
  }

  LightTriangles(triangles, world);
  TransformViewTriangles(triangles, world.GetActiveCamera(), picture->GetWidth(), picture->GetHeight());

  TurnTrianglesIntoPixels(picture, std::move(triangles), world);
}

}  // namespace renderer
