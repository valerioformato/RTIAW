#ifndef RTIAW_shapes_sphere
#define RTIAW_shapes_sphere

#include <memory>
#include <optional>

#include "App/Renderer/HitRecord.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Shapes {
class Sphere {
public:
  Sphere() = default;
  Sphere(point3 center, float radius) : m_center{center}, m_radius{radius}, m_sqRadius{m_radius * m_radius} {};

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  [[nodiscard]] std::optional<Shapes::AABB> BoundingBox(float time0, float time1) const;

public:
  point3 m_center{0, 0, 0};
  float m_radius{0};
  float m_sqRadius{0};
};
} // namespace RTIAW::Render::Shapes

#endif
