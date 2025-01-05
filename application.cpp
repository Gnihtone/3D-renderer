#include "application.h"

#include <iostream>

#include "components/visualizer.h"

void Application::Run() {
  Visualizer visualizer;

  auto& window = visualizer.GetWindow();
  glm::vec2 p1{100, 0}, p2{0, 100}, p3{200, 100};
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    visualizer.Visualize({Triangle2D{.points = {p1, p2, p3}, .color = Color{.r = 0, .g = 255, .b = 0, .a = 255}}});
  }
}
