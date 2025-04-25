#include "application.h"

#include <iostream>

#include "components/graphics.h"
#include "components/visualizer.h"
#include "objects/cube.h"

namespace renderer {

namespace {

float GetLength(const sf::Vector2i& vector) {
  return glm::sqrt(vector.x * vector.x + vector.y * vector.y);
}

}  // namespace

Application::Application()
    : world_(), runtime_(), visualizer_(runtime_.GetWindow()), graphics_pipeline_(), picture_() {}

void Application::Run() {
  auto cube = Cube();
  auto& cube_transform = cube.GetTransform();
  cube_transform.SetPosition({10, 5, 20});
  cube_transform.SetScale({4, 2, 2});

  {
    auto cube_child = Cube();
    auto& cube_child_transform = cube_child.GetTransform();
    cube_child_transform.SetPosition({5, 5, 5});
    cube_child_transform.SetScale({1, 1, 1});
    cube.AddChild(std::move(cube_child));
  }

  {
    auto cube2 = Cube();
    auto& cube2_transform = cube2.GetTransform();
    cube2_transform.SetPosition({10, 5, 70});
    cube2_transform.SetScale({2, 2, 2});
    world_.AddObject(std::move(cube2));
  }

  {
    Camera camera;
    auto& camera_transform = camera.GetTransform();
    camera_transform.SetRotation(Quaternion(Vector3(0, 0, 0)));
    camera_transform.SetPosition(Vector3{0, 0, 0});
    world_.AddCamera(std::move(camera));
  }

  {
    world_.AddLight(AmbientLight(Color(50, 50, 50, 255)));
    world_.AddLight(DirectionalLight(Color(255, 255, 255, 255), Vector3(1, 0, 1)));
  }

  auto& camera_ref = world_.GetActiveCamera();
  auto& camera_transform = camera_ref.GetTransform();
  auto& cube_ref = world_.AddObject(std::move(cube));

  Vector3 velocity{0};

  constexpr auto delta_radians_per_second = glm::radians(60.0f);
  constexpr auto delta_units_per_second = 10.0f;

  bool need_to_rotate = true;
  std::vector<bool> was_pressed(256, false);

  sf::Vector2i prev_mouse_pos = sf::Mouse::getPosition();
  sf::Vector2i mouse_pos_change(0, 0);

  auto on_event = [this, &need_to_rotate, &was_pressed, &velocity, &prev_mouse_pos,
                   &mouse_pos_change](const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
      runtime_.Stop();
    } else if (event.is<sf::Event::KeyPressed>()) {
      const auto key = event.getIf<sf::Event::KeyPressed>()->code;
      if (was_pressed[static_cast<int>(key)]) {
        return;
      }

      if (key == sf::Keyboard::Key::Escape) {
        runtime_.Stop();
      } else if (key == sf::Keyboard::Key::Space) {
        need_to_rotate = !need_to_rotate;
        velocity = Vector3{0};
      } else if (key == sf::Keyboard::Key::A) {
        velocity += Vector3{-1, 0, 0};
      } else if (key == sf::Keyboard::Key::D) {
        velocity += Vector3{1, 0, 0};
      } else if (key == sf::Keyboard::Key::W) {
        velocity += Vector3{0, 0, 1};
      } else if (key == sf::Keyboard::Key::S) {
        velocity += Vector3{0, 0, -1};
      } else if (key == sf::Keyboard::Key::I) {
        velocity += Vector3{0, 1, 0};
      } else if (key == sf::Keyboard::Key::K) {
        velocity += Vector3{0, -1, 0};
      }
      was_pressed[static_cast<int>(key)] = true;
    } else if (event.is<sf::Event::KeyReleased>()) {
      const auto key = event.getIf<sf::Event::KeyReleased>()->code;
      if (key == sf::Keyboard::Key::A) {
        velocity -= Vector3{-1, 0, 0};
      } else if (key == sf::Keyboard::Key::D) {
        velocity -= Vector3{1, 0, 0};
      } else if (key == sf::Keyboard::Key::W) {
        velocity -= Vector3{0, 0, 1};
      } else if (key == sf::Keyboard::Key::S) {
        velocity -= Vector3{0, 0, -1};
      } else if (key == sf::Keyboard::Key::I) {
        velocity -= Vector3{0, 1, 0};
      } else if (key == sf::Keyboard::Key::K) {
        velocity -= Vector3{0, -1, 0};
      }
      was_pressed[static_cast<int>(key)] = false;
    } else if (event.is<sf::Event::MouseMoved>()) {
      const auto new_mouse_pos = event.getIf<sf::Event::MouseMoved>()->position;

      mouse_pos_change = new_mouse_pos - prev_mouse_pos;
      prev_mouse_pos = new_mouse_pos;
    }
  };

  auto on_update = [this, &cube_ref, &camera_transform, &need_to_rotate, &velocity, &mouse_pos_change,
                    &prev_mouse_pos](float delta_time) {
    if (need_to_rotate) {
      auto& cube_transform = cube_ref.GetTransform();
      cube_transform.SetRotation(
          glm::rotate(cube_transform.GetRotation(), delta_radians_per_second * delta_time, Vector3(1.0f, 1.0f, 0.0f)));
    }

    const auto curr_rot = glm::normalize(camera_transform.GetRotation());
    auto forward = velocity * curr_rot;
    auto t = glm::length(velocity);
    std::cout << forward.x << ' ' << forward.y << ' ' << forward.z << std::endl;
    camera_transform.SetPosition(camera_transform.GetPosition() + t * forward * delta_units_per_second * delta_time);

    if (mouse_pos_change.x != 0 || mouse_pos_change.y != 0) {
      const auto axis = Vector3{-mouse_pos_change.y, -mouse_pos_change.x, 0} * curr_rot;
      camera_transform.SetRotation(glm::rotate(curr_rot, delta_radians_per_second * delta_time, axis));
      mouse_pos_change = {0, 0};
      prev_mouse_pos = {static_cast<int>(picture_.GetWidth() / 2), static_cast<int>(picture_.GetHeight() / 2)};
      sf::Mouse::setPosition(prev_mouse_pos, *runtime_.GetWindow());
    }

    graphics_pipeline_.Process(world_, &picture_);
    visualizer_.Visualize(picture_);
  };

  runtime_.Run(on_event, on_update);
}

}  // namespace renderer
