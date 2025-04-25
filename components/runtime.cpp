#include "runtime.h"

#include <functional>
#include <iostream>

#include "../common/types.h"

namespace renderer {

namespace {
Width kDefaultWidth{800};
Height kDefaultHeight{600};

}  // namespace

RunTime::RunTime() : window_(sf::VideoMode({kDefaultWidth, kDefaultHeight}), "Window") {}

void RunTime::Run(std::function<void(const sf::Event& event)> OnEvent, std::function<void(float delta_time)> OnUpdate) {
  sf::Clock clock;

  auto delta_time = clock.getElapsedTime().asSeconds();
  while (window_.isOpen()) {
    while (const std::optional event = window_.pollEvent()) {
      OnEvent(event.value());
    }

    OnUpdate(delta_time);
    delta_time = clock.getElapsedTime().asSeconds();
    clock.restart();
    std::cout << "FPS: " << 1.0f / delta_time << std::endl;
  }
}

sf::RenderWindow* RunTime::GetWindow() {
  return &window_;
}

void RunTime::Stop() {
  window_.close();
}

}  // namespace renderer
