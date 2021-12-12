#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;
  size_t closest_sphere_idx = -1;
  for (size_t sphere_idx = 0; sphere_idx < m_spheres.size(); ++sphere_idx) {
    const auto &sphere = m_spheres[sphere_idx];
    const vec3 oc = r.m_origin - sphere.m_center;
    const float half_b = glm::dot(oc, r.m_unit_direction);

    const float discriminant = half_b * half_b - glm::sq_length(oc) + sphere.m_radius * sphere.m_radius;
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

  if (closest_sphere_idx >= m_lambertian_offset)
    return m_lambertian_materials[closest_sphere_idx - m_lambertian_offset].Scatter(r, record);

  if (closest_sphere_idx >= m_metal_offset)
    return m_metal_materials[closest_sphere_idx - m_metal_offset].Scatter(r, record);

  return m_dielectric_materials[closest_sphere_idx].Scatter(r, record);
}
} // namespace RTIAW::Render
