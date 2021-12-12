#ifndef RTIAW_material_metal
#define RTIAW_material_metal

#include "HitRecord.h"
#include "ScatteringRecord.h"

namespace RTIAW::Render::Materials {
class Metal {
public:
  Metal(const color &albedo, float fuzzyness) : m_albedo(albedo), m_fuzzyness{fuzzyness < 1.0f ? fuzzyness : 1.0f} {}

  [[nodiscard]] ScatteringRecord Scatter(const Ray &r_in, const HitRecord &rec) const;

public:
  color m_albedo;
  float m_fuzzyness{1.0f};
};
} // namespace RTIAW::Render::Materials

#endif
