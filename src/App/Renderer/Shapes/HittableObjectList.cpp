#include <numeric>

#include "HittableObjectList.h"

namespace RTIAW::Render {
std::optional<HitRecord> HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  float closest_so_far = t_max;

  return std::accumulate(begin(objects), end(objects), std::optional<HitRecord>{},
                         [&](const auto &temp_value, const auto &object) {
                           if (auto temp_hit = object->Hit(r, t_min, closest_so_far); temp_hit) {
                             closest_so_far = temp_hit.value().t;
                             return temp_hit;
                           }
                           return temp_value;
                         });
}
} // namespace RTIAW::Render
