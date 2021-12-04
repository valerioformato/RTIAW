#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  HitResult result{};

  for (const auto &object : objects) {
    if (const auto &[temp_hit, scatter] = object.Hit(r, t_min, closest_so_far); temp_hit) {
      closest_so_far = temp_hit.value().t;
      result = {temp_hit, scatter};
    }
  }

  return result;
}
} // namespace RTIAW::Render
