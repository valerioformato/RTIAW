#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "HittableObject.h"
#include "Materials/Lambertian.h"

namespace RTIAW::Render::Materials {
ScatteringRecord Lambertian::Scatter(const Ray &r_in, const HitRecord &rec) const {
  vec3 scatter_direction = rec.normal + glm::sphericalRand(1.0f);

  // Catch degenerate scatter direction
  if (glm::any(glm::epsilonEqual(scatter_direction, vec3{0, 0, 0}, std::numeric_limits<float>::epsilon())))
    scatter_direction = rec.normal;

  return ScatteringRecord{m_albedo, Ray{rec.p, scatter_direction}};
}
} // namespace RTIAW::Render::Materials
