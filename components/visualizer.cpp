#include "visualizer.h"

#include <algorithm>
#include <queue>

#include "../common/geometry.h"

namespace {

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

}  // namespace

Visualizer::Visualizer()
    : window_(sf::VideoMode({WIDTH, HEIGHT}), "Window"), texture_(sf::Texture({WIDTH, HEIGHT})), sprite_(texture_) {
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
  texture_.update(reinterpret_cast<const uint8_t*>(pixels.data()), {WIDTH, HEIGHT}, {0, 0});

  window_.clear(sf::Color::White);
  window_.draw(sprite_);
  window_.display();
}
