#ifndef RTIAW_shapes_plane
#define RTIAW_shapes_plane

#include <array>
#include <optional>

#include "App/Renderer/HitRecord.h"
#include "App/Renderer/Shapes/AABB.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Shapes {
class Plane {
public:
  Plane() = default;
  Plane(const point3 &point, const float polar, const float azimuth);
  Plane(const point3 &point, glm::vec3 normal);
  explicit Plane(std::array<point3, 3> points);

  [[nodiscard]] inline float Polar() const { return m_polar; }
  [[nodiscard]] inline float Azimuth() const { return m_azimuth; }
  [[nodiscard]] glm::vec3 Normal() const { return m_normal; }

  [[nodiscard]] point3 Origin() const { return m_point; }

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  [[nodiscard]] std::optional<Shapes::AABB> BoundingBox(float time0, float time1) const { return std::nullopt; };

private:
  point3 m_point{};
  glm::vec3 m_normal{};
  float m_polar{};
  float m_azimuth{};

  size_t m_materialIndex{};
};
} // namespace RTIAW::Render::Shapes

#endif
