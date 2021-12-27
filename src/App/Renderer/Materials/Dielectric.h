#ifndef RTIAW_material_dielectric
#define RTIAW_material_dielectric

#include <optional>

#include "HitRecord.h"
#include "ScatteringRecord.h"

namespace RTIAW::Render::Materials {
class Dielectric {
  friend bool operator==(const Dielectric &lhs, const Dielectric &rhs);

public:
  explicit Dielectric(const float refractionIndex)
      : m_refractionIndex(refractionIndex), m_invRefractionIndex(1.0f / m_refractionIndex) {}

  [[nodiscard]] std::optional<ScatteringRecord> Scatter(const Ray &r_in, const HitRecord &rec) const;

private:
  float m_refractionIndex;
  float m_invRefractionIndex;

  static float Reflectance(const float cosine, const float refractionIndex) {
    // Use Schlick's approximation for reflectance.
    float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
  }
};

inline bool operator==(const Dielectric &lhs, const Dielectric &rhs) {
  return lhs.m_invRefractionIndex == rhs.m_refractionIndex;
}

} // namespace RTIAW::Render::Materials

#endif
