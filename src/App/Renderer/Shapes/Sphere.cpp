#include "Shapes/Sphere.h"

namespace RTIAW::Render::Shapes {
float Sphere::Hit(const Ray &r, const float t_min, const float t_max) const {

  const vec3 oc = r.m_origin - m_center;
  const float half_b = glm::dot(oc, r.m_unit_direction);
  const float c = glm::sq_length(oc) - m_radius * m_radius;

  const float discriminant = half_b * half_b - c;
  if (discriminant < 0)
    return t_max;
  const float sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  float root = -half_b - sqrtd;
  if (root < t_min || t_max < root) {
    root = -half_b + sqrtd;
    if (root < t_min || t_max < root)
      return t_max;
  }

  return root;
}

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
