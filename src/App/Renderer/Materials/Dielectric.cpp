#include <random>

#include "HittableObject.h"
#include "Materials/Dielectric.h"

// rng stuff
static std::mt19937 m_rnGenerator{};
static std::uniform_real_distribution<float> m_unifDistribution{0.0f, 1.0f};

namespace RTIAW::Render::Materials {
std::optional<ScatteringRecord> Dielectric::Scatter(const Ray &r_in, const HitRecord &rec) const {
  static constexpr color white{1.0f, 1.0f, 1.0f};
  const float refraction_ratio = rec.front_face ? m_invRefractionIndex : m_refractionIndex;

  const float cos_theta = -1.0f * glm::dot(r_in.direction, rec.normal);
  const float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

  if (const bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
      cannot_refract || Reflectance(cos_theta, refraction_ratio) > m_unifDistribution(m_rnGenerator)) {
    return ScatteringRecord{white, Ray{rec.p, glm::reflect(r_in.direction, rec.normal)}};
  } else {
    return ScatteringRecord{white, Ray{rec.p, glm::refract(r_in.direction, rec.normal, refraction_ratio)}};
  }
}
} // namespace RTIAW::Render::Materials
