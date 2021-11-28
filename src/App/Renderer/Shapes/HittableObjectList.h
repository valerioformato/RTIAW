#ifndef RTIAW_shapes_hittablelist
#define RTIAW_shapes_hittablelist

#include <memory>
#include <vector>

#include "Shapes/HittableObject.h"

namespace RTIAW::Render {
class HittableObjectList : public HittableObject {
public:
  HittableObjectList() = default;
  explicit HittableObjectList(std::shared_ptr<HittableObject> object) { Add(object); }

  void Clear() { objects.clear(); }
  void Add(std::shared_ptr<HittableObject> object) { objects.push_back(std::move(object)); }

  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, float t_min, float t_max) const override;

public:
  std::vector<std::shared_ptr<HittableObject>> objects;
};
} // namespace RTIAW::Render

#endif
