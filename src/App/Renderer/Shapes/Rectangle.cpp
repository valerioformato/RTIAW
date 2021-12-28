#include "Shapes/Rectangle.h"

#include <iostream>
#include <stdexcept>

namespace RTIAW::Render::Shapes {
Rectangle::Rectangle(std::array<point3, 3> points) : Parallelogram(points) {
  if (glm::dot(m_vertices[0], m_vertices[1]) < 2 * std::numeric_limits<float>::epsilon()) {
    m_plane = Shapes::Plane(points[0], glm::cross(m_vertices[0], m_vertices[1]));
  } else {
    throw std::runtime_error("Rectangle::Rectangle: the given points do not define a rectangle.");
  }
}
} // namespace RTIAW::Render::Shapes
