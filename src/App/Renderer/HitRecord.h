#ifndef RTIAW_hitrecord
#define RTIAW_hitrecord

#include <memory>

#include "Ray.h"

namespace RTIAW::Render {
struct HitRecord {
  // HitRecord(const float tValue, point3 point) : t{tValue}, p{point} {}

  float t;
  point3 p;
  vec3 normal;
  bool front_face;

  void SetFaceNormal(const Ray &r, const vec3 &outward_normal) {
    front_face = glm::dot(r.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

} // namespace RTIAW::Render
#endif
