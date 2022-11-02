#ifndef RTIAW_shapes_aabb
#define RTIAW_shapes_aabb

#include <optional>

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
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

private:
  point3 m_a;
  point3 m_b;
};
} // namespace RTIAW::Render::Shapes

#endif