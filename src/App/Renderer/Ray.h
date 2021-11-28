#ifndef RTIAW_ray
#define RTIAW_ray

#include <utility>

#include "Utils.h"

namespace RTIAW::Render {
class Ray {
public:
  Ray() = default;
  Ray(point3 origin, vec3 direction) : m_origin{origin}, m_direction{direction} {}

  [[nodiscard]] point3 origin() const { return m_origin; }
  [[nodiscard]] vec3 direction() const { return m_direction; }

  [[nodiscard]] point3 at(float t) const { return m_origin + m_direction * t; }

private:
  point3 m_origin;
  vec3 m_direction;
};
} // namespace RTIAW::Render

#endif
