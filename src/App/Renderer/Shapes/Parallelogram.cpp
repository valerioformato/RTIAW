#include "App/Renderer/Shapes/Parallelogram.h"

#include <stdexcept>

namespace RTIAW::Render::Shapes {
Parallelogram::Parallelogram(const std::array<point3, 3> points) {
  bool isValid = false;
  if (points[0] != points[1] && points[1] != points[2] && points[2] != points[0]) {
    // Assume points[0] as the position of the plane, and compute the position of the two vertices relative to it
    m_vertices[0] = points[1] - points[0];
    m_vertices[1] = points[2] - points[0];
    if (std::abs(glm::dot(glm::normalize(m_vertices[0]), glm::normalize(m_vertices[1]))) != 1.0f) {
      isValid = true;
      m_plane = Shapes::Plane(points[0], glm::cross(m_vertices[0], m_vertices[1]));
    }
  }
  if (!isValid) {
    throw std::runtime_error("Parallelogram::Parallelogram: the given points do not define a parallelogram.");
  }
}

std::array<point3, 4> Parallelogram::Vertices() const {
  const point3 point0 = m_plane.Origin();
  return {point0, point0 + m_vertices[0], point0 + m_vertices[1], point0 + m_vertices[0] + m_vertices[1]};
}

float Parallelogram::FastHit(const Ray &r, const float t_min, const float t_max) const {
  if (const auto t = m_plane.FastHit(r, t_min, t_max); t < std::numeric_limits<float>::max()) {
    const auto intersection = r.At(t);

    // intersection point w.r.t. parallelogram origin
    const auto k = intersection - m_plane.Origin();
    // const float den = (m_vertices[1].y * m_vertices[0].x - m_vertices[1].x * m_vertices[0].y);
    // const float proj0 = (k.x * m_vertices[1].y - k.y * m_vertices[1].x) / den;
    // const float proj1 = (k.y * m_vertices[0].x - k.x * m_vertices[0].y) / den;
    // if (proj0 > 0 && proj1 > 0 && proj0 < 1.0f && proj1 < 1.0f) {
    //   return t;
    // }

    float udv = glm::dot(m_vertices[0], m_vertices[1]);
    float xdu = glm::dot(k, m_vertices[0]);
    float xdv = glm::dot(k, m_vertices[1]);
    float udu = glm::dot(m_vertices[0], m_vertices[0]);
    float vdv = glm::dot(m_vertices[1], m_vertices[1]);

    float d = udv * udv - udu * vdv;

    float a = (udv * xdv - xdu * vdv) / d;
    float b = (udv * xdu - xdv * udu) / d;

    if (a >= 0 && a < 1 && b >= 0 && b < 1)
      return t;
  }

  return std::numeric_limits<float>::max();
}

HitRecord Parallelogram::ComputeHitRecord(const Ray &r, const float t) const {
  return {t, r.At(t), m_plane.Normal(), glm::dot(m_plane.Normal(), r.direction) < 0};
}

tl::optional<HitRecord> Parallelogram::Hit(const Ray &r, const float t_min, const float t_max) const {
  static constexpr tl::optional<HitRecord> empty_result{};

  if (const auto t = FastHit(r, t_min, t_max); t < std::numeric_limits<float>::max()) {
    return ComputeHitRecord(r, t);
  } else {
    return empty_result;
  }
}

tl::optional<Shapes::AABB> Parallelogram::BoundingBox(float time0, float time1) const {
  point3 a{std::numeric_limits<decltype(point3::x)>::max()};
  point3 b{std::numeric_limits<decltype(point3::x)>::lowest()};

  for (const auto &vertex : Vertices()) {
    a = glm::min(a, vertex);
    b = glm::max(b, vertex);
  }

  return AABB{a, b};
}
} // namespace RTIAW::Render::Shapes
