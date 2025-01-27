#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../common/geometry.h"

class Visualizer {
 public:
  Visualizer();
  ~Visualizer();

  Visualizer(const uint32_t width, const uint32_t height);

  sf::RenderWindow& GetWindow();
  void Visualize(const std::vector<Color>& pixels);

 private:
  uint32_t width_;
  uint32_t height_;

  sf::RenderWindow window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};
