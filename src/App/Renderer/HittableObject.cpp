#include "HittableObject.h"
#include "HittableObjectList.h"

namespace RTIAW::Render {
float HittableObject::FastHit(const Ray &r, float t_min, float t_max) const {
  return std::visit(
      overloaded{
          [&](const auto &shape) { return shape.FastHit(r, t_min, t_max); },
      },
      m_shape);
}

HitRecord HittableObject::ComputeHitRecord(const Ray &r, float t) const {
  static constexpr std::optional<HitRecord> empty_result{};

  return std::visit(
      overloaded{
          [&](const auto &shape) { return shape.ComputeHitRecord(r, t); },
      },
      m_shape);
}

std::optional<HitRecord> HittableObject::Hit(const Ray &r, float t_min, float t_max) const {
  return std::visit(
      overloaded{
          [&](const auto &shape) { return shape.Hit(r, t_min, t_max); },
      },
      m_shape);
}
std::optional<Shapes::AABB> HittableObject::BoundingBox(float time0, float time1) const {
  return std::visit(
      overloaded{
          [&](const auto &shape) { return shape.BoundingBox(time0, time1); },
      },
      m_shape);
}
} // namespace RTIAW::Render
