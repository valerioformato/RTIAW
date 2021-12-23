#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  HitResult result{};

  const HittableObject *closest_obj = nullptr;

  for (const auto &object : objects) {
    if (const auto &temp_hit = object.Hit(r, t_min, closest_so_far); temp_hit) {
      closest_obj = &object;
      closest_so_far = temp_hit.value().t;
      result.first = temp_hit;
    }
  }

  if (closest_obj) {
    result.second = closest_obj->Scatter(r, result.first.value());
  }

  return result;
}
} // namespace RTIAW::Render
