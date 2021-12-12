#ifndef RTIAW_ray
#define RTIAW_ray

#include "Utils.h"

namespace RTIAW::Render {
struct Ray {
  constexpr Ray() = default;
  constexpr Ray(point3 origin, vec3 direction) : m_origin{origin}, m_direction{direction} {}

  [[nodiscard]] constexpr point3 At(float t) const { return m_origin + m_direction * t; }

  point3 m_origin{0, 0, 0};
  vec3 m_direction{0, 0, 0};
};
} // namespace RTIAW::Render

#endif
