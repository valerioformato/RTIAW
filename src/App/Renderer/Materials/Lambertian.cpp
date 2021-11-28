#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "Materials/Lambertian.h"
#include "Shapes/HittableObject.h"

namespace RTIAW::Render::Materials {
std::optional<ScatteringResult> Lambertian::Scatter(const Ray &r_in, const HitRecord &rec) const {
  vec3 scatter_direction = rec.normal + glm::sphericalRand(1.0f);

  // Catch degenerate scatter direction
  if (glm::any(glm::epsilonEqual(scatter_direction, vec3{0, 0, 0}, std::numeric_limits<float>::epsilon())))
    scatter_direction = rec.normal;

  return ScatteringResult{m_albedo, Ray{rec.p, scatter_direction}};
}
} // namespace RTIAW::Render::Materials
