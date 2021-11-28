#ifndef RTIAW_material
#define RTIAW_material

#include <optional>

#include "Ray.h"
#include "Utils.h"

namespace RTIAW::Render {
struct HitRecord;

struct ScatteringResult {
  color attenuation;
  Ray ray;
};

class Material {
public:
  [[nodiscard]] virtual std::optional<ScatteringResult> Scatter(const Ray &r_in, const HitRecord &rec) const = 0;
};
} // namespace RTIAW::Render

#endif
