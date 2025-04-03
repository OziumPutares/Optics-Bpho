#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <imgui.h>// necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include <imgui-SFML.h>// for ImGui::SFML::* functions and SFML-specific overloads


int main()
{

  sf::Vector2u screenResolution = { 640, 480 };// NOLINT
  sf::RenderWindow window(sf::VideoMode{ screenResolution }, "ImGui + SFML = <3");
  const auto frameRate = 60;
  window.setFramerateLimit(frameRate);
  if (!ImGui::SFML::Init(window)) { return -1; }

  sf::CircleShape shape(100.F);
  shape.setFillColor(sf::Color::Green);

  sf::Clock deltaClock;
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      if (event->is<sf::Event::Closed>()) { window.close(); }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::ShowDemoWindow();

    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    window.clear();
    window.draw(shape);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}
