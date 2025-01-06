#include "visualizer.h"

#include <algorithm>
#include <queue>

#include "../common/geometry.h"

Visualizer::Visualizer()
    : width_(800),
      height_(600),
      window_(sf::VideoMode({width_, height_}), "Window"),
      texture_(sf::Texture({width_, height_})),
      sprite_(texture_) {
  window_.setVerticalSyncEnabled(false);
  sprite_.setPosition({0, 0});
}

Visualizer::~Visualizer() {
  window_.close();
}

sf::RenderWindow& Visualizer::GetWindow() {
  return window_;
}

void Visualizer::Visualize(const std::vector<Color>& pixels) {
  texture_.update(reinterpret_cast<const uint8_t*>(pixels.data()), {width_, height_}, {0, 0});

  window_.clear(sf::Color::White);
  window_.draw(sprite_);
  window_.display();
}

Visualizer::Visualizer(const uint32_t width, const uint32_t height)
    : width_(width),
      height_(height),
      window_(sf::VideoMode({width, height}), "Window"),
      texture_(sf::Texture({width, height})),
      sprite_(texture_) {}
