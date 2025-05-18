#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../common/picture.h"

namespace renderer {

class Visualizer {
 public:
  explicit Visualizer(sf::RenderWindow* render_window);

  void Visualize(const Picture& picture);

 private:
  sf::RenderWindow* window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};

}  // namespace renderer
