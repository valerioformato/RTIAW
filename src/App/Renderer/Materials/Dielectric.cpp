#include <random>

#include "Materials/Dielectric.h"
#include "Shapes/HittableObject.h"

// rng stuff
static std::mt19937 m_rnGenerator{};
static std::uniform_real_distribution<float> m_unifDistribution{0.0f, 1.0f};

namespace RTIAW::Render::Materials {
std::optional<ScatteringResult> Dielectric::Scatter(const Ray &r_in, const HitRecord &rec) const {
  float refraction_ratio = rec.front_face ? (1.0f / m_refractionIndex) : m_refractionIndex;

  const vec3 unit_direction = glm::normalize(r_in.Direction());
  float cos_theta = std::min(glm::dot(-unit_direction, rec.normal), 1.0f);
  float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

  bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
  vec3 direction;

  if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > m_unifDistribution(m_rnGenerator)) {
    direction = glm::reflect(unit_direction, rec.normal);
  } else {
    direction = glm::refract(unit_direction, rec.normal, refraction_ratio);
  }

  return ScatteringResult{color{1.0f, 1.0f, 1.0f}, Ray{rec.p, direction}};
}
} // namespace RTIAW::Render::Materials
