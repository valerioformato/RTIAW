#include "App/Renderer/Shapes/MovingSphere.h"
#include "AABB.h"

template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

namespace RTIAW::Render::Shapes {
point3 MovingSphere::Center(float time) const {
  return m_center_start + ((time - m_time_start) / (m_time_end - m_time_start)) * (m_center_end - m_center_start);
}

float MovingSphere::FastHit(const Ray &r, const float t_min, const float t_max) const {
  const vec3 oc = r.origin - Center(r.time);

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

HitRecord MovingSphere::ComputeHitRecord(const Ray &r, const float t) const {
  HitRecord result{};
  result.t = t;
  result.p = r.At(result.t);
  vec3 outward_normal = glm::normalize(result.p - Center(r.time));

  // NOTE: I really don't like this, the book author is using negative radius to artificially invert all normals of the
  // sphere, so he can model hollow spheres...
  outward_normal *= sign(m_radius);

  result.SetFaceNormal(r, outward_normal);

  return result;
}

tl::optional<HitRecord> MovingSphere::Hit(const Ray &r, const float t_min, const float t_max) const {
  static constexpr tl::optional<HitRecord> empty_result{};

  if (const auto t = FastHit(r, t_min, t_max); t < std::numeric_limits<float>::max()) {
    return ComputeHitRecord(r, t);
  } else {
    return empty_result;
  }
}

tl::optional<Shapes::AABB> MovingSphere::BoundingBox(float time0, float time1) const {
  AABB box0(Center(time0) - vec3(m_radius, m_radius, m_radius), Center(time0) + vec3(m_radius, m_radius, m_radius));
  AABB box1(Center(time1) - vec3(m_radius, m_radius, m_radius), Center(time1) + vec3(m_radius, m_radius, m_radius));
  return SurroundingBox(box0, box1);
}
} // namespace RTIAW::Render::Shapes