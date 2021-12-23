#include "Shapes/Sphere.h"

namespace RTIAW::Render::Shapes {
float Sphere::FastHit(const Ray &r, const float t_min, const float t_max) const {
  const vec3 oc = r.origin - m_center;
  const float a = glm::sq_length(r.direction);
  const float half_b = glm::dot(oc, r.direction);
  const float c = glm::sq_length(oc) - m_radius * m_radius;

  const float discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return std::numeric_limits<float>::max();
  const float sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  float root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return std::numeric_limits<float>::max();
  }

  return root;
}

HitRecord Sphere::ComputeHitRecord(const Ray &r, const float t) const {
  HitRecord result{};
  result.t = t;
  result.p = r.At(result.t);
  vec3 outward_normal = glm::normalize(result.p - m_center);

  // NOTE: I really don't like this, the book author is using negative radius to artificially invert all normals of the
  // sphere, so he can model hollow spheres...
  if (m_radius < 0)
    outward_normal *= -1.0f;

  result.SetFaceNormal(r, outward_normal);

  return result;
}

std::optional<HitRecord> Sphere::Hit(const Ray &r, const float t_min, const float t_max) const {
  static constexpr std::optional<HitRecord> empty_result{};

  const auto t = FastHit(r, t_min, t_max);
  if (t < std::numeric_limits<float>::max()) {
    HitRecord result{};
    result.t = t;
    result.p = r.At(result.t);
    vec3 outward_normal = glm::normalize(result.p - m_center);

    // NOTE: I really don't like this, the book author is using negative radius to artificially invert all normals of
    // the sphere, so he can model hollow spheres...
    if (m_radius < 0)
      outward_normal *= -1.0f;

    result.SetFaceNormal(r, outward_normal);

    return result;
  } else {
    return empty_result;
  }
}
} // namespace RTIAW::Render::Shapes
