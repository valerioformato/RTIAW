#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  std::optional<ScatteringRecord> result;
  HitRecord record;
  record.t = t_max;

  const DielectricSphere *closest_dielectric = nullptr;
  for (const auto &object : m_dielectrics) {
    object.m_sphere.Hit(record, r, t_min, closest_so_far);
    if (record.t < closest_so_far) // valid, i.e. closest
    {
      closest_so_far = record.t;
      closest_dielectric = &object;
    }
  }
  if (closest_dielectric)
    result = closest_dielectric->m_material.Scatter(r, record);


  const LambertianSphere *closest_lambertian = nullptr;
  for (const auto &object : m_lambertians) {
    object.m_sphere.Hit(record, r, t_min, closest_so_far);
    if (record.t < closest_so_far) // valid, i.e. closest
    {
      closest_so_far = record.t;
      closest_lambertian = &object;
    }
  }
  if (closest_lambertian)
    result = closest_lambertian->m_material.Scatter(r, record);


  const MetalSphere *closest_metal = nullptr;
  for (const auto &object : m_metals) {
    object.m_sphere.Hit(record, r, t_min, closest_so_far);
    if (record.t < closest_so_far) // valid, i.e. closest
    {
      closest_so_far = record.t;
      closest_metal = &object;
    }
  }
  if (closest_metal)
    result = closest_metal->m_material.Scatter(r, record);

  return result;
}
} // namespace RTIAW::Render
