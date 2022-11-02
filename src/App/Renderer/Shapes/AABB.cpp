#include "App/Renderer/Shapes/AABB.h"
float RTIAW::Render::Shapes::AABB::FastHit(const RTIAW::Render::Ray &r, const float t_min, const float t_max) const {
  float tM = t_max;
  float tm = t_min;
  for (int a = 0; a < 3; a++) {
    auto invD = 1.0f / r.direction[a];
    auto t0 = (m_a[a] - r.origin[a]) * invD;
    auto t1 = (m_b[a] - r.origin[a]) * invD;
    if (invD < 0.0f)
      std::swap(t0, t1);
    tm = t0 > tm ? t0 : tm;
    tM = t1 < tM ? t1 : tM;
    if (tM <= tm)
      return std::numeric_limits<float>::max();
  }

  return tm;
}
