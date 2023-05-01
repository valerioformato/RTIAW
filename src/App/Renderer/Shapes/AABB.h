#ifndef RTIAW_shapes_aabb
#define RTIAW_shapes_aabb

#include <tl/optional.hpp>

#include "App/Renderer/HitRecord.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Shapes {

class AABB {
public:
  AABB() = default;
  AABB(point3 a, point3 b) : m_a{a}, m_b{b} {}

  [[nodiscard]] point3 A() const { return m_a; }
  [[nodiscard]] point3 B() const { return m_b; }

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;

private:
  point3 m_a;
  point3 m_b;
};

static inline AABB SurroundingBox(const AABB &box0, const AABB &box1) {
  point3 small(glm::min(box0.A(), box1.A()));
  point3 big(glm::max(box0.B(), box1.B()));

  return AABB{small, big};
}

} // namespace RTIAW::Render::Shapes

#endif