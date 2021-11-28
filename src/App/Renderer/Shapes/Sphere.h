#ifndef RTIAW_shapes_sphere
#define RTIAW_shapes_sphere

#include "Shapes/HittableObject.h"
#include "Utils.h"

namespace RTIAW::Render::Shapes {
class Sphere : public HittableObject {
public:
  Sphere() = default;
  Sphere(point3 center, float radius, std::shared_ptr<Material> material)
      : m_center{center}, m_radius{radius}, m_material{std::move(material)} {};

  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, float t_min, float t_max) const override;

public:
  point3 m_center{0, 0, 0};
  float m_radius{0};
  std::shared_ptr<Material> m_material;
};
} // namespace RTIAW::Render::Shapes

#endif
