#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  static constexpr HitResult empty_result{};

  float closest_t = t_max;
  const HittableObject *closest_obj = nullptr;

  for (const auto &object : objects) {
    if (const float temp_t = object.FastHit(r, t_min, closest_t); temp_t < std::numeric_limits<float>::max()) {
      closest_obj = &object;
      closest_t = temp_t;
    }
  }

  if (!closest_obj) {
    return empty_result;
  } else {
    const auto hitr = closest_obj->ComputeHitRecord(r, closest_t);
    return {hitr, closest_obj->Scatter(r, hitr)};
  }
}
} // namespace RTIAW::Render
