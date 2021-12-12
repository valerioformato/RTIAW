#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  HitResult result{};

  for (const auto &dielectric : m_dielectrics) {
    HitRecord record = dielectric.m_sphere.Hit(r, t_min, closest_so_far);
    if (record.t != -1) // valid, i.e. closest
    {
      closest_so_far = record.t;
      result = {record, dielectric.m_material.Scatter(r, record)};
    }
  }

  for (const auto &lambertian : m_lambertians) {
    HitRecord record = lambertian.m_sphere.Hit(r, t_min, closest_so_far);
    if (record.t != -1) // valid, i.e. closest
    {
      closest_so_far = record.t;
      result = {record, lambertian.m_material.Scatter(r, record)};
    }
  }

  for (const auto &metal : m_metals) {
    HitRecord record = metal.m_sphere.Hit(r, t_min, closest_so_far);
    if (record.t != -1) // valid, i.e. closest
    {
      closest_so_far = record.t;
      result = {record, metal.m_material.Scatter(r, record)};
    }
  }

  return result;
}
} // namespace RTIAW::Render
