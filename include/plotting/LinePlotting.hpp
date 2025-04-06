#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <ranges>
#include <span>
#include <vector>
namespace plotting {

template<template<typename> typename Range>
concept PlottableRange = requires(Range<int> range) {
  std::begin(range);
  std::begin(range)++;
  (static_cast<bool>(std::begin(range) != std::end(range)));
};
class PlotConcept : sf::Drawable
{
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};
template<typename T>
concept PlottableValue = requires(T val) { static_cast<double>(val); };
template<typename T>
  requires PlottableValue<T>
class LinePlot : PlotConcept
{
  std::span<T> m_XCooridnates;
  T m_MaxXCoordinate = std::ranges::max(m_XCooridnates);
  std::span<T> m_YCooridnates;
  T m_MaxYCoordinate = std::ranges::max(m_YCooridnates);
  std::optional<float> m_XToYRatio;

public:
  LinePlot(std::span<T> xCooridnates, std::span<T> yCooridnates)
    : m_XCooridnates(xCooridnates), m_YCooridnates(yCooridnates)
  {}
  void draw(sf::RenderTarget &target, [[maybe_unused]] sf::RenderStates states) const override
  {
    auto XScalingFactor = target.getSize().x / m_MaxXCoordinate;
    auto YScalingFactor = target.getSize().y / m_MaxYCoordinate;
    if (m_XToYRatio.has_value()) {
      if (static_cast<float>(target.getSize().x) * *m_XToYRatio >= static_cast<float>(target.getSize().y)) {
        YScalingFactor = XScalingFactor * *m_XToYRatio;
      } else {
        XScalingFactor = YScalingFactor * (1 / (*m_XToYRatio));
      }
    }

    auto ListOfPointsForLine =
      sf::VertexArray(sf::PrimitiveType::Lines, std::size(m_XCooridnates) + std::size(m_YCooridnates));
    for (auto [xCoord1, yCoord1] : std::ranges::views::zip(m_XCooridnates, m_YCooridnates)) {
      ListOfPointsForLine.append(sf::Vertex{ .position = { m_XCooridnates, m_YCooridnates }, .color = sf::Color::Red });
    }
  }
};
}// namespace plotting
