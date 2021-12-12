#include "Shapes/Sphere.h"

namespace RTIAW::Render::Shapes {
HitRecord Sphere::GetHitRecord(float root, const Ray &r) const {
  HitRecord record;
  record.t = root;
  record.p = r.At(record.t);

  vec3 outward_normal = glm::normalize(record.p - m_center);

  // NOTE: I really don't like this, the book author is using negative radius to artificially invert all normals of the
  // sphere, so he can model hollow spheres...
  if (m_radius < 0)
    outward_normal *= -1.0f;

  record.SetFaceNormal(r, outward_normal);
  return record;
}

} // namespace RTIAW::Render::Shapes
