#ifndef RTIAW_material_lambertian
#define RTIAW_material_lambertian

#include "Materials/Material.h"

namespace RTIAW::Render::Materials {
class Lambertian : public Material {
public:
  Lambertian(const color &albedo) : m_albedo(albedo) {}

  [[nodiscard]] std::optional<ScatteringResult> Scatter(const Ray &r_in, const HitRecord &rec) const override;

public:
  color m_albedo;
};
} // namespace RTIAW::Render::Materials

#endif
