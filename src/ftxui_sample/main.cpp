#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <array>
#include <imgui-SFML.h>// for ImGui::SFML::* functions and SFML-specific overloads
#include <imgui.h>// necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h


int main()
{
  sf::ContextSettings Settings;
  // NOLINTNEXTLINE
  Settings.antiAliasingLevel = 100;

  sf::Vector2u screenResolution = { 640, 480 };// NOLINT
  sf::RenderWindow Window(sf::VideoMode{ screenResolution }, "ImGui + SFML = <3");
  auto const FrameRate = 60;
  Window.setFramerateLimit(FrameRate);
  if (!ImGui::SFML::Init(Window)) { return -1; }

  sf::CircleShape Shape(100.F);
  Shape.setFillColor(sf::Color::Green);
  // NOLINTBEGIN
  std::array<sf::Vector2f, 4> ListOfLinesCoords = {
    sf::Vector2f{ 10, 12 }, sf::Vector2f{ 12, 10 }, sf::Vector2f{ 32, 32 }, sf::Vector2f{ 32, 12 }
  };
  // NOLINTEND
  auto ListOfLines = sf::VertexArray(sf::PrimitiveType::LineStrip);
  for (auto Coords : ListOfLinesCoords) {
    // NOLINTNEXTLINE
    ListOfLines.append(sf::Vertex{ .position{ Coords.x * 10, Coords.y * 10 }, .color = sf::Color::Cyan });
  }


  sf::Clock DeltaClock;
  while (Window.isOpen()) {
    while (auto const Event = Window.pollEvent()) {
      ImGui::SFML::ProcessEvent(Window, *Event);

      if (Event->is<sf::Event::Closed>()) { Window.close(); }
    }

    ImGui::SFML::Update(Window, DeltaClock.restart());

    ImGui::ShowDemoWindow();

    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    Window.clear();
    Window.draw(Shape);
    Window.draw(ListOfLines);
    ImGui::SFML::Render(Window);
    Window.display();
  }

  ImGui::SFML::Shutdown();
}
