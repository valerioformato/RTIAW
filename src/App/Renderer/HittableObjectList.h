#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <vector>

#include "HittableObject.h"

namespace RTIAW::Render {
class HittableObjectList {
public:
  HittableObjectList() = default;

  void Clear() { objects.clear(); }
  void Add(const HittableObject &object) { objects.push_back(object); }
  void Add(HittableObject &&object) { objects.push_back(object); }

  template <typename... Args> void Construct(Args &&...args) { objects.emplace_back(std::forward<Args>(args)...); }

  [[nodiscard]] HitResult Hit(const Ray &r, float t_min, float t_max) const;

public:
  std::vector<HittableObject> objects;
};
} // namespace RTIAW::Render

#endif
