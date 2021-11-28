#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "Materials/Metal.h"
#include "Shapes/HittableObject.h"

namespace RTIAW::Render::Materials {
std::optional<ScatteringResult> Metal::Scatter(const Ray &r_in, const HitRecord &rec) const {
  vec3 reflected = glm::reflect(glm::normalize(r_in.Direction()), rec.normal);
  vec3 new_direction = reflected + m_fuzzyness * glm::sphericalRand(1.0f);

  if (glm::dot(new_direction, rec.normal) > 0) {
    return ScatteringResult{m_albedo, Ray{rec.p, new_direction}};
  }

  return std::optional<ScatteringResult>{};
}
} // namespace RTIAW::Render::Materials
