#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

#include "App/Renderer/HittableObject.h"
#include "App/Renderer/Materials/Metal.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Materials {
tl::optional<ScatteringRecord> Metal::Scatter(const Ray &r_in, const HitRecord &rec) const {
  static constexpr tl::optional<ScatteringRecord> empty_result{};

  if (const vec3 new_direction = glm::reflect(r_in.direction, rec.normal) + m_fuzzyness * Random::sphericalRand(1.0f);
      glm::dot(new_direction, rec.normal) > 0) {
    return ScatteringRecord{m_albedo, Ray{rec.p, new_direction, r_in.time}};
  }

  return empty_result;
}
} // namespace RTIAW::Render::Materials
