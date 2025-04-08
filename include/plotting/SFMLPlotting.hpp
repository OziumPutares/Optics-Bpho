
#include "LinePlotting.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <functional>
#include <ranges>
namespace plotting {
template<> class PlotConcept<void, sf::RenderTarget &, sf::RenderStates> : public sf::Drawable
{
public:
  PlotConcept(PlotConcept const &) = default;
  PlotConcept(PlotConcept &&) = default;
  PlotConcept &operator=(PlotConcept const &) = default;
  PlotConcept &operator=(PlotConcept &&) = default;
  ~PlotConcept() override = default;
  [[nodiscard]] virtual std::unique_ptr<PlotConcept> clone() const = 0;
};

using SFMLPlotConcept = PlotConcept<void, sf::RenderTarget &, sf::RenderStates>;

template<typename T>
  requires PlottableValue<T>
class ConcreteLinePlot<T, void, void, sf::RenderTarget &, sf::RenderStates> : SFMLPlotConcept
{
  std::span<T> m_XCooridnates;
  T m_MaxXCoordinate = std::ranges::max(m_XCooridnates);
  std::span<T> m_YCooridnates;
  T m_MaxYCoordinate = std::ranges::max(m_YCooridnates);


private:
  std::function<Colour(T, T)> m_ColouringStrategy;
  Vector<float, 2> m_Size;
  Vector<float, 2> m_Position;
  [[nodiscard]] std::unique_ptr<SFMLPlotConcept> clone() const override { return ConcreteLinePlot{ *this }; }
  void draw(sf::RenderTarget &target, sf::RenderStates states)
  {
    auto XScale = m_Size.points[0] / m_MaxXCoordinate;
    auto YScale = m_Size.points[1] / m_MaxYCoordinate;
    auto PlotLine = sf::VertexArray(sf::PrimitiveType::LineStrip);
    for (auto &&[xCoord, yCoord] : std::ranges::views::zip(m_XCooridnates, m_YCooridnates)) {
      PlotLine.append(
        sf::Vertex{ .position = { xCoord * XScale, yCoord * YScale }, .color = m_ColouringStrategy(xCoord, yCoord) });
    }
    target.draw(PlotLine, states);
  }
};

}// namespace plotting
