#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../common/geometry.h"

class Visualizer {
 public:
  Visualizer();
  ~Visualizer();

  sf::RenderWindow& GetWindow();
  void Visualize(const std::vector<Color>& pixels);

 private:
  sf::RenderWindow window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};
