#include "HittableObjectList.h"

// helper type for the visitor #4
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace RTIAW::Render {
float HittableObject::FastHit(const Ray &r, float t_min, float t_max) const {
  const auto t = std::visit(
      overloaded{
          [&](const Shapes::Sphere &sphere) { return sphere.FastHit(r, t_min, t_max); },
      },
      m_shape);

  if (!t)
    return std::numeric_limits<float>::max();

  return t;
}

HitRecord HittableObject::ComputeHitRecord(const Ray &r, float t) const {
  static constexpr std::optional<HitRecord> empty_result{};

  return std::visit(
      overloaded{
          [&](const Shapes::Sphere &sphere) { return sphere.ComputeHitRecord(r, t); },
      },
      m_shape);
}

std::optional<HitRecord> HittableObject::Hit(const Ray &r, float t_min, float t_max) const {
  static constexpr std::optional<HitRecord> empty_result{};

  const auto hitr = std::visit(
      overloaded{
          [&](const Shapes::Sphere &sphere) { return sphere.Hit(r, t_min, t_max); },
      },
      m_shape);

  if (!hitr)
    return empty_result;

  return hitr;
}

std::optional<ScatteringRecord> HittableObject::Scatter(const Ray &r, const HitRecord &rec) const {
  return std::visit(overloaded{
                        [&](const Materials::Lambertian &lambertian) { return lambertian.Scatter(r, rec); },
                        [&](const Materials::Metal &metal) { return metal.Scatter(r, rec); },
                        [&](const Materials::Dielectric &dielectric) { return dielectric.Scatter(r, rec); },
                    },
                    m_material);
}
} // namespace RTIAW::Render
