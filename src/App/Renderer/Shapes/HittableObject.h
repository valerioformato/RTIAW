#ifndef RTIAW_shapes_hittable
#define RTIAW_shapes_hittable

#include <memory>
#include <optional>

#include "Ray.h"

namespace RTIAW::Render {
class Material;

struct HitRecord {
  point3 p;
  vec3 normal;
  std::shared_ptr<Material> mat_ptr;
  float t;
  bool front_face;

  inline void SetFaceNormal(const Ray &r, const vec3 &outward_normal) {
    front_face = glm::dot(r.Direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class HittableObject {
public:
  // HittableObject() = default;
  virtual ~HittableObject() = default;

  [[nodiscard]] virtual std::optional<HitRecord> Hit(const Ray &r, float t_min, float t_max) const = 0;
};
} // namespace RTIAW::Render

#endif
