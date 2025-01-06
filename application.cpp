#include "application.h"

#include <iostream>

#include "components/graphics.h"
#include "components/visualizer.h"

void Application::Run() {
  Visualizer visualizer;
  GraphicsPipeline graphics_pipeline;

  auto& window = visualizer.GetWindow();
  glm::vec2 p1{0, 0}, p2{0, 100}, p3{100, 0};
  glm::vec2 p4{0, 100}, p5{100, 0}, p6{100, 100};
  std::vector<Triangle2D> triangles;
  for (int i = 0; i < 1; i++) {
    triangles.push_back(Triangle2D{p1, p2, p3, Color::RED});
    triangles.push_back(Triangle2D{p4, p5, p6, Color::RED});
  }
  while (window.isOpen()) {
    const auto start = std::chrono::steady_clock::now();
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::KeyPressed>()) {
        const auto key = event->getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    TurnTrianglesIntoPixels(800, 600, triangles);
    const auto pixels = graphics_pipeline.Process({});
    visualizer.Visualize(pixels);
    std::cout << "FPS: " << 1000000000. / (std::chrono::steady_clock::now() - start).count() << std::endl;
  }
}
