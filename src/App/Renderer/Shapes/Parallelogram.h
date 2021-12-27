#ifndef RTIAW_shapes_parallelogram
#define RTIAW_shapes_parallelogram

#include <optional>

#include "HitRecord.h"
#include "Utils.h"

namespace RTIAW::Render::Shapes {

class Parallelogram {

public:
  Parallelogram() = default;
  virtual ~Parallelogram() = default;

  Parallelogram(std::array<Point, 3> points);

  Herd::Plane Plane() const;

  std::array<Point, 4> Vertices() const;

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

protected:
  Plane m_plane;                  // The origin of this plane lies on one vertex of the parallelogram
  std::array<vec3, 2> m_vertices; // Coordinates of two vertices w.r.t. the origin of the plane

#ifdef HS_USE_ROOT
  ClassDef(Parallelogram, 1)
#endif
};
} // namespace RTIAW::Render::Shapes

#endif
