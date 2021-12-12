#ifndef RTIAW_material_lambertian
#define RTIAW_material_lambertian

#include "HitRecord.h"
#include "ScatteringRecord.h"

namespace RTIAW::Render::Materials {
class Lambertian {
public:
  Lambertian(const color &albedo) : m_albedo(albedo) {}

  [[nodiscard]] ScatteringRecord Scatter(const Ray &, const HitRecord &rec) const;

public:
  color m_albedo;
};
} // namespace RTIAW::Render::Materials

#endif
