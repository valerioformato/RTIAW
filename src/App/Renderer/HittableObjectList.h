#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <vector>
#include <mutex>

#include "BVH.h"

#include "HittableObject.h"

namespace RTIAW::Render {
class HittableObjectList {
public:
  HittableObjectList() = default;

  void Clear() { m_objects.clear(); }
  void Add(const Shape &shape, const Material &material);

  [[nodiscard]] HitResult Hit(const Ray &r, float t_min, float t_max) const;

  [[nodiscard]] const std::vector<HittableObject> &Objects() const { return m_objects; }

private:
  std::vector<HittableObject> m_objects;
  std::vector<Material> materials;

  mutable std::mutex m_bvhMutex;
  mutable bool m_bvhIsValid{false};
  mutable BVHNode m_bvh;

  void UpdateBVH(float t_min, float t_max) const;
};
} // namespace RTIAW::Render

#endif
