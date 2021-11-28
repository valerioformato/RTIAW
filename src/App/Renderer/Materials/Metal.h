#ifndef RTIAW_material_metal
#define RTIAW_material_metal

#include "Materials/Material.h"

namespace RTIAW::Render::Materials {
class Metal : public Material {
public:
  Metal(const color &albedo, float fuzzyness) : m_albedo(albedo), m_fuzzyness{fuzzyness < 1.0f ? fuzzyness : 1.0f} {}

  [[nodiscard]] std::optional<ScatteringResult> Scatter(const Ray &r_in, const HitRecord &rec) const override;

public:
  color m_albedo;
  float m_fuzzyness{1.0f};
};
} // namespace RTIAW::Render::Materials

#endif
