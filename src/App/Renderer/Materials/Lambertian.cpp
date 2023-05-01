#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "App/Renderer/HittableObject.h"
#include "App/Renderer/Materials/Lambertian.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Materials {
tl::optional<ScatteringRecord> Lambertian::Scatter(const Ray &r_in, const HitRecord &rec) const {
  vec3 scatter_direction = rec.normal + Random::sphericalRand(1.0f);

  // Catch degenerate scatter direction
  if (glm::any(glm::epsilonEqual(scatter_direction, vec3{0, 0, 0}, std::numeric_limits<float>::epsilon())))
    scatter_direction = rec.normal;

  return ScatteringRecord{m_albedo, Ray{rec.p, scatter_direction, r_in.time}};
}
} // namespace RTIAW::Render::Materials
