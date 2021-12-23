#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "HittableObject.h"
#include "Materials/Metal.h"

namespace RTIAW::Render::Materials {
std::optional<ScatteringRecord> Metal::Scatter(const Ray &r_in, const HitRecord &rec) const {
  vec3 reflected = glm::reflect(r_in.m_direction, rec.normal);
  vec3 new_direction = reflected + m_fuzzyness * glm::sphericalRand(1.0f);

  if (glm::dot(new_direction, rec.normal) > 0) {
    return ScatteringRecord{m_albedo, Ray{rec.p, new_direction}};
  }

  return std::optional<ScatteringRecord>{};
}
} // namespace RTIAW::Render::Materials
