#ifndef RTIAW_shapes_sphere
#define RTIAW_shapes_sphere

#include <memory>
#include <optional>

#include "HitRecord.h"
#include "Utils.h"

namespace RTIAW::Render::Shapes {
class Sphere {
public:
  Sphere() = default;
  Sphere(point3 center, float radius) : m_center{center}, m_radius{radius} {};

  float Hit(const Ray &r, const float t_min, const float t_max) const;

  HitRecord GetHitRecord(float root, const Ray &r) const;

public:
  point3 m_center{0, 0, 0};
  float m_radius{0};
};
} // namespace RTIAW::Render::Shapes

#endif
