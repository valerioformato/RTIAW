#ifndef RTIAW_ray
#define RTIAW_ray

#include "Utils.h"

namespace RTIAW::Render {
class Ray {
public:
  constexpr Ray() = default;
  Ray(point3 origin, vec3 direction) : origin{origin}, direction{glm::normalize(direction)} {}

  point3 origin{0, 0, 0};
  vec3 direction{0, 0, 0};

  [[nodiscard]] constexpr point3 At(float t) const { return origin + direction * t; }

private:
};
} // namespace RTIAW::Render

#endif
