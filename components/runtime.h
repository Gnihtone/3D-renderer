#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <functional>

namespace renderer {

class RunTime {
 public:
  RunTime();

  void Run(std::function<void(const sf::Event& event)> OnEvent, std::function<void(float delta_time)> OnUpdate);
  sf::RenderWindow* GetWindow();

  void Stop();

 private:
  sf::RenderWindow window_;
};

}  // namespace renderer
