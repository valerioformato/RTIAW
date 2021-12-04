#ifndef RTIAW_hitrecord
#define RTIAW_hitrecord

#include <memory>

#include "Ray.h"

namespace RTIAW::Render {
struct HitRecord {
  point3 p;
  vec3 normal;
  float t;
  bool front_face;

  inline void SetFaceNormal(const Ray &r, const vec3 &outward_normal) {
    front_face = glm::dot(r.Direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

} // namespace RTIAW::Render
#endif
