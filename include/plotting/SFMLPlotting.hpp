
#include "LinePlotting.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
namespace plotting {
template<>
class PlotConcept<void, sf::RenderTarget &, sf::RenderStates>
  : public sf::Drawable
  , public sf::Transformable
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
template<typename T, typename DrawStrat>
class ConcreteLinePlot<T, DrawStrat, void, sf::RenderTarget &, sf::RenderTarget> : SFMLPlotConcept
{
};

}// namespace plotting
