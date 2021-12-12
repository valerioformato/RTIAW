#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  std::optional<ScatteringRecord> result;

  const auto hit_spheres = [&](const auto &spheres) -> size_t {
    size_t closest_sphere_idx = -1;
    for (size_t sphere_idx = 0; sphere_idx < spheres.size(); ++sphere_idx) {
      const auto &sphere = spheres[sphere_idx];
      const vec3 oc = r.m_origin - sphere.m_center;
      const float half_b = glm::dot(oc, r.m_unit_direction);

      const float discriminant =
          half_b * half_b - glm::sq_length(oc) + sphere.m_radius * sphere.m_radius;
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
    return closest_sphere_idx;
  };


  const auto lambertian_idx = hit_spheres(m_lambertian_spheres);
  if (lambertian_idx != size_t(-1)) {
    const auto record = m_lambertian_spheres[lambertian_idx].GetHitRecord(closest_so_far, r);
    result = m_lambertian_materials[lambertian_idx].Scatter(r, record);
  }

  const auto dielectric_idx = hit_spheres(m_dielectric_spheres);
  if (dielectric_idx != size_t(-1)) {
    const auto record = m_dielectric_spheres[dielectric_idx].GetHitRecord(closest_so_far, r);
    result = m_dielectric_materials[dielectric_idx].Scatter(r, record);
  }

  const auto metal_idx = hit_spheres(m_metal_spheres);
  if (metal_idx != size_t(-1)) {
    const auto record = m_metal_spheres[metal_idx].GetHitRecord(closest_so_far, r);
    result = m_metal_materials[metal_idx].Scatter(r, record);
  }

  return result;
}
} // namespace RTIAW::Render
