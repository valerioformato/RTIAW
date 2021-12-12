#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <vector>

#include "HittableObject.h"

namespace RTIAW::Render {
using HitResult = std::pair<HitRecord, ScatteringRecord>;

class HittableObjectList {
public:
  HittableObjectList() = default;

  inline void Clear() {
    m_dielectrics.clear();
    m_lambertians.clear();
    m_metals.clear();
  }

  inline void Add(Shapes::Sphere sphere, Materials::Dielectric material) { m_dielectrics.emplace_back(sphere, material); }
  inline void Add(Shapes::Sphere sphere, Materials::Lambertian material) { m_lambertians.emplace_back(sphere, material); }
  inline void Add(Shapes::Sphere sphere, Materials::Metal material) { m_metals.emplace_back(sphere, material); }

  [[nodiscard]] HitResult Hit(const Ray &r, float t_min, float t_max) const;

private:
  std::vector<DielectricSphere> m_dielectrics;
  std::vector<LambertianSphere> m_lambertians;
  std::vector<MetalSphere> m_metals;
};
} // namespace RTIAW::Render

#endif
