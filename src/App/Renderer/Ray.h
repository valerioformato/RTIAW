#ifndef RTIAW_ray
#define RTIAW_ray

#include "Utils.h"

namespace RTIAW::Render {
class Ray {
public:
  constexpr Ray() = default;
  Ray(const point3 origin, const vec3 direction, const float time)
      : origin{origin}, direction{glm::normalize(direction)}, time{time} {}

  point3 origin{0, 0, 0};
  vec3 direction{0, 0, 0};
  float time{0};

  [[nodiscard]] constexpr point3 At(const float t) const { return origin + direction * t; }
};
} // namespace RTIAW::Render

#endif
