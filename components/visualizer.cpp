#include "visualizer.h"

#include <algorithm>
#include <queue>

#include "../common/geometry.h"

namespace renderer {

namespace {

constexpr Width kDefaultWidth{800};
constexpr Height kDefaultHeight{600};

const uint8_t* ConvertPixels(const Color* pixels) {
  return reinterpret_cast<const uint8_t*>(pixels);
}

}  // namespace

Visualizer::Visualizer(sf::RenderWindow* render_window)
    : window_(render_window), texture_(sf::Texture({kDefaultWidth, kDefaultHeight})), sprite_(texture_) {
  window_->setMouseCursorVisible(false);
  window_->setMouseCursorGrabbed(true);
}

void Visualizer::Visualize(const Picture& picture) {
  const sf::Vector2u size{picture.GetWidth(), picture.GetHeight()};
  if (texture_.getSize() != size) {
    window_->setSize(size);
    const auto is_resize_successful = texture_.resize(size);
    assert(is_resize_successful);
  }
  texture_.update(ConvertPixels(picture.GetPixels()), size, {0, 0});

  window_->clear(sf::Color::White);
  window_->draw(sprite_);
  window_->display();
}

}  // namespace renderer
