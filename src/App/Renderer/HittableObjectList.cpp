#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  std::optional<ScatteringRecord> result;

  const auto hit_spheres = [&](const auto &objects) {
    size_t closest_obj_idx = -1;
    for (size_t obj_idx = 0; obj_idx < objects.size(); ++obj_idx) {
      const auto &object = objects[obj_idx];
      const vec3 oc = r.m_origin - object.m_sphere.m_center;
      const float half_b = glm::dot(oc, r.m_unit_direction);

      const float discriminant =
          half_b * half_b - glm::sq_length(oc) + object.m_sphere.m_radius * object.m_sphere.m_radius;
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
        closest_obj_idx = obj_idx;
      }
    }
    if (closest_obj_idx != size_t(-1)) {
      const auto &object = objects[closest_obj_idx];
      const auto record = object.m_sphere.GetHitRecord(closest_so_far, r);
      result = object.m_material.Scatter(r, record);
    }
  };

  hit_spheres(m_lambertians);
  hit_spheres(m_dielectrics);
  hit_spheres(m_metals);

  return result;
}
} // namespace RTIAW::Render
