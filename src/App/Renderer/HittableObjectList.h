#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <vector>

#include "HittableObject.h"

namespace RTIAW::Render {
class HittableObjectList {
public:
  HittableObjectList() = default;

  void Clear() { m_objects.clear(); }
  void Add(const Shape &shape, const Material &material);
  // void Add(const HittableObject &object) { m_objects.push_back(object); }
  // void Add(HittableObject &&object) { m_objects.push_back(object); }

  // template <typename... Args> void Construct(Args &&...args) { m_objects.emplace_back(std::forward<Args>(args)...); }

  [[nodiscard]] HitResult Hit(const Ray &r, float t_min, float t_max) const;

private:
  std::vector<HittableObject> m_objects;
  std::vector<Material> materials;
};
} // namespace RTIAW::Render

#endif
