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
#include <concepts>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <vector>
namespace plotting {

struct Colour
{
  std::uint8_t red{};
  std::uint8_t green{};
  std::uint8_t blue{};
  std::uint8_t alpha = std::numeric_limits<std::uint8_t>::max();
};
template<typename T>
concept PlottableValue = requires(T val) { static_cast<double>(val); };
template<template<typename> typename Range, typename T>
concept ValidRange = requires(Range<T> &&range) {
  std::begin(range);
  std::end(range);
  std::begin(range)++;
  std::begin(range)--;
  (static_cast<bool>(std::begin(range) != std::end(range)));
  *std::begin(range);
  std::size(range)->std::size_t;
};
template<typename Ret, typename... Params> class PlotConcept
{
public:
  PlotConcept(PlotConcept const &) = default;
  PlotConcept(PlotConcept &&) = default;
  PlotConcept &operator=(PlotConcept const &) = default;
  PlotConcept &operator=(PlotConcept &&) = default;
  virtual ~PlotConcept() = default;
  virtual Ret draw(Params...) = 0;
  [[nodiscard]] virtual std::unique_ptr<PlotConcept> clone() const = 0;

private:
};

// Actual implementation
template<typename T, typename DrawStrat, typename Ret, typename... Params>
  requires PlottableValue<T> && std::invocable<DrawStrat, Ret, Params...>
class ConcreteLinePlot : PlotConcept<Ret, Params...>
{
  std::span<T> m_XCooridnates;
  T m_MaxXCoordinate = std::ranges::max(m_XCooridnates);
  std::span<T> m_YCooridnates;
  T m_MaxYCoordinate = std::ranges::max(m_YCooridnates);
  std::optional<float> m_XToYRatio;
  std::function<Colour(T, T)> m_ColouringStrategy;
  DrawStrat m_DrawStrategy;

public:
  [[nodiscard]] virtual std::unique_ptr<PlotConcept<Ret, Params...>> clone() const
  {
    return ConcreteLinePlot{ .m_XCooridnates = m_XCooridnates,
      .m_MaxXCoordinate = m_MaxXCoordinate,
      .m_YCooridnates = m_YCooridnates,
      .m_MaxYCoordinate = m_MaxYCoordinate,
      .m_XToYRatio = m_XToYRatio,
      .m_ColouringStrategy = m_ColouringStrategy,
      .draw = m_DrawStrategy };
  }
  ConcreteLinePlot(
    std::span<T> xCooridnates,
    std::span<T> yCooridnates,
    DrawStrat drawingStrategy,
    std::function<Colour(T, T)> colouringStrategy =
      [](T, T) { return Colour{ .red = std::numeric_limits<std::uint8_t>::max() }; })
    : m_XCooridnates(xCooridnates), m_YCooridnates(yCooridnates), m_ColouringStrategy(colouringStrategy),
      m_DrawStrategy(std::move(drawingStrategy))
  {}

  virtual Ret draw(Params... parameters) { return m_DrawStrategy(parameters...); }
};
}// namespace plotting
