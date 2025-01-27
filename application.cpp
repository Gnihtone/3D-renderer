#include "application.h"

#include <iostream>

#include "components/graphics.h"
#include "components/visualizer.h"

namespace {
const std::vector<glm::vec3> cube_vertices = {
    {-0.5, -0.5, -0.5},  // FRONT-BOTTOM-LEFT
    {0.5, -0.5, -0.5},   // FRONT-BOTTOM-RIGHT
    {0.5, 0.5, -0.5},    // FRONT-TOP-RIGHT
    {-0.5, 0.5, -0.5},   // FRONT-TOP-LEFT

    {-0.5, -0.5, 0.5},  // BACK-BOTTOM-LEFT
    {0.5, -0.5, 0.5},   // BACK-BOTTOM-RIGHT
    {0.5, 0.5, 0.5},    // BACK-TOP-RIGHT
    {-0.5, 0.5, 0.5}    // BACK-TOP-LEFT
};

const std::vector<Triangle3D> cube_triangles = {
    // FRONT
    {.points = {cube_vertices[0], cube_vertices[2], cube_vertices[1]}, .color = Color::RED},
    {.points = {cube_vertices[0], cube_vertices[2], cube_vertices[3]}, .color = Color::RED},

    // LEFT
    {.points = {cube_vertices[0], cube_vertices[3], cube_vertices[4]}, .color = Color::BLUE},
    {.points = {cube_vertices[7], cube_vertices[3], cube_vertices[4]}, .color = Color::BLUE},

    // RIGHT
    {.points = {cube_vertices[1], cube_vertices[2], cube_vertices[5]}, .color = Color::GREEN},
    {.points = {cube_vertices[6], cube_vertices[2], cube_vertices[5]}, .color = Color::GREEN},

    // TOP
    {.points = {cube_vertices[2], cube_vertices[3], cube_vertices[6]}, .color = Color::WHITE},
    {.points = {cube_vertices[7], cube_vertices[3], cube_vertices[6]}, .color = Color::WHITE},

    // BOTTOM
    {.points = {cube_vertices[0], cube_vertices[1], cube_vertices[4]}, .color = Color::YELLOW},
    {.points = {cube_vertices[5], cube_vertices[1], cube_vertices[4]}, .color = Color::YELLOW},

    // BACK
    {.points = {cube_vertices[4], cube_vertices[5], cube_vertices[6]}, .color = Color::PURPLE},
    {.points = {cube_vertices[4], cube_vertices[7], cube_vertices[6]}, .color = Color::PURPLE},
};

}  // namespace

void Application::Run() {
  const uint32_t width = 800;
  const uint32_t height = 600;

  Visualizer visualizer(width, height);
  GraphicsPipeline graphics_pipeline;

  auto cube = std::make_shared<Object>(cube_triangles, glm::vec3(5, 5, 20));
  World world;
  world.AddObject(cube);

  cube->SetScale({4, 2, 2});

  sf::Clock clock;

  const auto delta_radians_per_second = glm::radians(60.0f);

  auto delta_time = clock.getElapsedTime().asSeconds();
  auto& window = visualizer.GetWindow();
  while (window.isOpen()) {
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

    cube->SetRotation(
        glm::rotate(cube->GetRotation(), delta_radians_per_second * delta_time, glm::vec3(1.0f, 1.0f, 0.0f)));

    const auto pixels = graphics_pipeline.Process(world, width, height);
    visualizer.Visualize(pixels);
    delta_time = clock.getElapsedTime().asSeconds();
    clock.restart();
    std::cout << "FPS: " << 1.0f / delta_time << std::endl;
  }
}
