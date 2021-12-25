#include "HittableObjectList.h"

// helper type for the visitor #4
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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

std::optional<ScatteringRecord> HittableObject::Scatter(const Ray &r, const HitRecord &rec) const {
  return std::visit(
      overloaded{
          [&](const auto &material) { return material.Scatter(r, rec); },
      },
      m_material);
}
} // namespace RTIAW::Render
