#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <imgui-SFML.h>// for ImGui::SFML::* functions and SFML-specific overloads
#include <imgui.h>// necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h


int main()
{

  sf::Vector2u screenResolution = { 640, 480 };// NOLINT
  sf::RenderWindow Window(sf::VideoMode{ screenResolution }, "ImGui + SFML = <3");
  const auto FrameRate = 60;
  Window.setFramerateLimit(FrameRate);
  if (!ImGui::SFML::Init(Window)) { return -1; }

  sf::CircleShape Shape(100.F);
  Shape.setFillColor(sf::Color::Green);
  // NOLINTNEXTLINE
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;


  sf::Clock DeltaClock;
  while (Window.isOpen()) {
    while (const auto Event = Window.pollEvent()) {
      ImGui::SFML::ProcessEvent(Window, *Event);

      if (Event->is<sf::Event::Closed>()) { Window.close(); }
    }

    ImGui::SFML::Update(Window, DeltaClock.restart());

    ImGui::Begin("Challenge 1");

    ImGui::End();

    Window.clear();
    Window.draw(Shape);
    ImGui::SFML::Render(Window);
    if ((0 != (ImGuiConfigFlags_ViewportsEnable & static_cast<unsigned int>(ImGui::GetIO().ConfigFlags)))) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }
    Window.display();
  }

  ImGui::SFML::Shutdown();
}
