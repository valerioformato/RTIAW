#ifndef RTIAW_ray
#define RTIAW_ray

#include "Utils.h"

namespace RTIAW::Render {
struct Ray {
  constexpr Ray() = default;
  Ray(point3 origin, vec3 direction) : m_origin{origin}, m_unit_direction{glm::normalize(direction)} {}

  [[nodiscard]] constexpr point3 At(float t) const { return m_origin + m_unit_direction * t; }

  point3 m_origin{0, 0, 0};
  vec3 m_unit_direction{0, 0, 0};
};
} // namespace RTIAW::Render

#endif
