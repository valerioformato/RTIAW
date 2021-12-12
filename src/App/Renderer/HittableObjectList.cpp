#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<ScatteringRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  std::optional<ScatteringRecord> result;

  const DielectricSphere *closest_dielectric = nullptr;
  for (const auto &object : m_dielectrics) {
    const auto proximity = object.m_sphere.Hit(r, t_min, closest_so_far);
    if (proximity < closest_so_far)
    {
      closest_so_far = proximity;
      closest_dielectric = &object;
    }
  }
  if (closest_dielectric) {
    const auto record = closest_dielectric->m_sphere.GetHitRecord(closest_so_far, r);
    result = closest_dielectric->m_material.Scatter(r, record);
  }

  const LambertianSphere *closest_lambertian = nullptr;
  for (const auto &object : m_lambertians) {
    const auto proximity = object.m_sphere.Hit(r, t_min, closest_so_far);
    if (proximity < closest_so_far)
    {
      closest_so_far = proximity;
      closest_lambertian = &object;
    }
  }
  if (closest_lambertian) {
    const auto record = closest_lambertian->m_sphere.GetHitRecord(closest_so_far, r);
    result = closest_lambertian->m_material.Scatter(r, record);
  }

  const MetalSphere *closest_metal = nullptr;
  for (const auto &object : m_metals) {
    const auto proximity = object.m_sphere.Hit(r, t_min, closest_so_far);
    if (proximity < closest_so_far) {
      closest_so_far = proximity;
      closest_metal = &object;
    }
  }
  if (closest_metal) {
    const auto record = closest_metal->m_sphere.GetHitRecord(closest_so_far, r);
    result = closest_metal->m_material.Scatter(r, record);
  }

  return result;
}
} // namespace RTIAW::Render
