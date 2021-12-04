#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<HitRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  std::optional<HitRecord> result{};

  for (const auto &object : objects) {
    if (auto temp_hit = object->Hit(r, t_min, closest_so_far); temp_hit) {
      closest_so_far = temp_hit.value().t;
      result = temp_hit;
    }
  }

  return result;
}
} // namespace RTIAW::Render
