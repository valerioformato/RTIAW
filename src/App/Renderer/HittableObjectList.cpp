#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  static constexpr HitResult empty_result{};

  const HittableObject *closest_obj = nullptr;

  for (const auto &object : objects) {
    if (const auto &temp_t = object.FastHit(r, t_min, closest_so_far); temp_t < std::numeric_limits<float>::max()) {
      closest_obj = &object;
      closest_so_far = temp_t;
    }
  }

  if (!closest_obj) {
    return empty_result;
  } else {
    const auto hitr = closest_obj->ComputeHitRecord(r, closest_so_far);
    return {hitr, closest_obj->Scatter(r, hitr)};
  }
}
} // namespace RTIAW::Render
