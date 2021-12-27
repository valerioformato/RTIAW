#ifndef RTIAW_shapes_parallelogram
#define RTIAW_shapes_parallelogram

#include <optional>

#include "HitRecord.h"
#include "Utils.h"

#include "Shapes/Plane.h"

namespace RTIAW::Render::Shapes {

class Parallelogram {
public:
  Parallelogram() = default;
  virtual ~Parallelogram() = default;

  explicit Parallelogram(std::array<point3, 3> points);

  [[nodiscard]] Shapes::Plane Plane() const { return m_plane; };

  [[nodiscard]] std::array<point3, 4> Vertices() const;

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

protected:
  Shapes::Plane m_plane{};          // The origin of this plane lies on one vertex of the parallelogram
  std::array<vec3, 2> m_vertices{}; // Coordinates of two vertices w.r.t. the origin of the plane
};
} // namespace RTIAW::Render::Shapes

#endif
