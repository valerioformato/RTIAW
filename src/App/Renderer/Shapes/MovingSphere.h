#ifndef RTIAW_shapes_movingsphere
#define RTIAW_shapes_movingsphere

#include <memory>
#include <optional>

#include "App/Renderer/HitRecord.h"
#include "App/Renderer/Utils.h"

namespace RTIAW::Render::Shapes {
class MovingSphere {
public:
  MovingSphere() = default;
  MovingSphere(point3 center_start, point3 center_end, float time_start, float time_end, float radius)
      : m_center_start{center_start}, m_center_end{center_end}, m_time_start{time_start},
        m_time_end{time_end}, m_radius{radius}, m_sqRadius{m_radius * m_radius} {};

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] HitRecord ComputeHitRecord(const Ray &r, const float t) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  point3 Center(float time) const;

public:
  point3 m_center_start{0, 0, 0};
  point3 m_center_end{0, 0, 0};
  float m_time_start{0};
  float m_time_end{0};
  float m_radius{0};
  float m_sqRadius{0};
};
} // namespace RTIAW::Render::Shapes

#endif