#include <spdlog/spdlog.h>

#include "App/Renderer/Shapes/Sphere.h"

template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

namespace RTIAW::Render::Shapes {
float Sphere::FastHit(const Ray &r, const float t_min, const float t_max) const {
  const vec3 oc = r.origin - m_center;

  // In the following computations we solve a quadratic equation in the usual way
  // However the 2nd order coefficient "a" is the length of the ray direction, which
  // is 1 by construction. If the following computations don't look right, it's because
  // "a" is optimized away :)
  // I'll leave the original version commented out...

  // const float a = glm::dot(r.direction, r.direction); // 1 by construction!
  const float half_b = glm::dot(oc, r.direction);
  // const float c = glm::dot(oc, oc) - m_radius * m_radius;

  // const float discriminant = half_b * half_b - a * c;
  const float discriminant = half_b * half_b - glm::dot(oc, oc) + m_sqRadius;
  if (discriminant < 0)
    return std::numeric_limits<float>::max();
  const float sqrtd = std::sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  float root = -half_b - sqrtd;
  if (root < t_min || t_max < root) {
    root = -half_b + sqrtd;
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
  outward_normal *= sign(m_radius);

  result.SetFaceNormal(r, outward_normal);

  return result;
}

std::optional<HitRecord> Sphere::Hit(const Ray &r, const float t_min, const float t_max) const {
  static constexpr std::optional<HitRecord> empty_result{};

  const auto t = FastHit(r, t_min, t_max);
  if (t < std::numeric_limits<float>::max()) {
    return ComputeHitRecord(r, t);
  } else {
    return empty_result;
  }
}
} // namespace RTIAW::Render::Shapes
