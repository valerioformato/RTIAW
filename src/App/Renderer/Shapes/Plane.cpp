#include "App/Renderer/Shapes/Plane.h"

#include <stdexcept>

namespace RTIAW::Render::Shapes {
Plane::Plane(const point3 &point, const float polar, const float azimuth)
    : m_point(point), m_polar(polar), m_azimuth(azimuth) {
  m_normal = {std::sin(m_polar) * std::cos(m_azimuth), std::sin(m_polar) * std::sin(m_azimuth),
              std::cos(Utils::pi - m_polar)};
}

Plane::Plane(const point3 &point, const glm::vec3 normal)
    : m_point(point), m_polar(glm::polar(normal)), m_azimuth(glm::azimuth(normal)), m_normal{normal} {}

Plane::Plane(std::array<point3, 3> points) {
  bool isPlane = false;
  if (points[0] != points[1] && points[1] != points[2] && points[2] != points[0]) {
    // Assume points[0] as the position of the plane, and compute the position of the two vertices relative to it
    std::array<glm::vec3, 2> vertices; // Coordinates of two vertices w.r.t. the origin of the plane
    vertices[0] = points[1] - points[0];
    vertices[1] = points[2] - points[0];
    glm::vec3 normal = glm::cross(vertices[0], vertices[1]);
    // Check that the two vectors are not colinear.
    if (normal != glm::vec3(0, 0, 0)) {
      isPlane = true;
      m_point = points[0];
      m_polar = glm::polar(normal);
      m_azimuth = glm::azimuth(normal);
    }
  }
  if (!isPlane) {
    throw std::runtime_error("Plane::Plane: the given points do not define a plane.");
  }
}

float Plane::FastHit(const Ray &r, const float t_min, const float t_max) const {
  const float d_dot_n = glm::dot(r.direction, m_normal);

  if (std::abs(d_dot_n) < 2 * std::numeric_limits<float>::epsilon()) {
    return std::numeric_limits<float>::max();
  }

  const float t = glm::dot(m_point - r.origin, m_normal) / d_dot_n;
  if (t < t_min || t_max < t) {
    return std::numeric_limits<float>::max();
  }

  return t;
}

HitRecord Plane::ComputeHitRecord(const Ray &r, const float t) const {
  return {t, r.At(t), m_normal, glm::dot(m_normal, r.direction) < 0};
}

std::optional<HitRecord> Plane::Hit(const Ray &r, const float t_min, const float t_max) const {
  static constexpr std::optional<HitRecord> empty_result{};

  if (const auto t = FastHit(r, t_min, t_max); t < std::numeric_limits<float>::max()) {
    return ComputeHitRecord(r, t);
  } else {
    return empty_result;
  }
}
} // namespace RTIAW::Render::Shapes
