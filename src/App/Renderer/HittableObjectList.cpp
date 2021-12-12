#include <numeric>

#include "HittableObjectList.h"

// helper type for the visitor #4
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;
  size_t closest_sphere_idx = -1;
  for (size_t sphere_idx = 0; sphere_idx < m_spheres.size(); ++sphere_idx) {
    const auto &sphere = m_spheres[sphere_idx];
    const vec3 oc = r.m_origin - sphere.m_center;

    // prune if sphere is too far away from ray
    const auto cp = oc.x * r.m_unit_direction.y - oc.y * r.m_unit_direction.x;
    if (std::abs(cp) > sphere.m_radius)
      continue;

    const float half_b = glm::dot(oc, r.m_unit_direction);

    const float discriminant = half_b * half_b - glm::dot(oc, oc) + sphere.m_radius * sphere.m_radius;
    if (discriminant < 0.f)
      continue;
    const float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = -half_b - sqrtd;
    if (root < t_min || t_max < root) {
      root = -half_b + sqrtd;
      if (root < t_min || t_max < root)
        continue;
    }

    if (root < closest_so_far) {
      closest_so_far = root;
      closest_sphere_idx = sphere_idx;
    }
  }

  if (closest_sphere_idx == size_t(-1))
    return {};

  const auto record = m_spheres[closest_sphere_idx].GetHitRecord(closest_so_far, r);

  return std::visit(overloaded{
                        [&](const Materials::Lambertian &lambertian) { return lambertian.Scatter(r, record); },
                        [&](const Materials::Metal &metal) { return metal.Scatter(r, record); },
                        [&](const Materials::Dielectric &dielectric) { return dielectric.Scatter(r, record); },
                    },
                    m_materials[closest_sphere_idx]);
}
} // namespace RTIAW::Render
