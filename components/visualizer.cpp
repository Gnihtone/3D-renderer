#include "visualizer.h"

#include <algorithm>
#include <queue>

#include <glm/glm.hpp>

#include "../common/geometry.h"

namespace {

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

uint32_t ColorToRawBytes(const Color& color) {
  return *reinterpret_cast<const uint32_t*>(&color);
}

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

}  // namespace

Visualizer::Visualizer()
    : window_(sf::VideoMode({WIDTH, HEIGHT}), "Window"), texture_(sf::Texture({WIDTH, HEIGHT})), sprite_(texture_) {
  window_.setVerticalSyncEnabled(true);
}

Visualizer::~Visualizer() {
  window_.close();
}

sf::RenderWindow& Visualizer::GetWindow() {
  return window_;
}

void Visualizer::Visualize(const std::vector<Triangle2D>& triangles) {
  sprite_.setPosition({0, 0});

  std::vector<uint32_t> color_map(WIDTH * HEIGHT, 0);

  for (const auto& triangle : triangles) {
    const auto top_most_point = FindTopMostPoint(triangle);
    int current_min_x = std::max(0.f, std::min(top_most_point.x, WIDTH - 1.f));
    int current_max_x = current_min_x;

    const auto max_y = std::min(GetMaxY(triangle) + 1, static_cast<float>(HEIGHT));

    for (int y = std::max(top_most_point.y, 0.f); y < max_y; ++y) {
      while (current_min_x > 0 && CheckPointIsInTriangle(triangle, current_min_x, y)) {
        --current_min_x;
      }
      while (current_min_x < WIDTH - 1 && !CheckPointIsInTriangle(triangle, current_min_x, y)) {
        ++current_min_x;
        if (current_max_x < current_min_x) {
          current_max_x = current_min_x;
        }
      }
      while (current_max_x < WIDTH - 1 && CheckPointIsInTriangle(triangle, current_max_x, y)) {
        ++current_max_x;
      }
      while (current_max_x > 0 && !CheckPointIsInTriangle(triangle, current_max_x, y)) {
        --current_max_x;
        if (current_min_x > current_max_x) {
          current_min_x = current_max_x;
        }
      }

      for (int x = current_min_x; x <= current_max_x; ++x) {
        color_map[y * WIDTH + x] = ColorToRawBytes(triangle.color);
      }
    }
  }

  texture_.update(reinterpret_cast<uint8_t*>(color_map.data()), {WIDTH, HEIGHT}, {0, 0});

  window_.clear(sf::Color::White);
  window_.draw(sprite_);
  window_.display();
}
