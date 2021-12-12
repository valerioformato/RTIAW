#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <optional>
#include <vector>

#include "Materials/Materials.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {

class HittableObjectList {
public:
  HittableObjectList() = default;

  inline void Clear() {
    m_spheres.clear();
    m_materials.clear();
  }

  inline void Add(Shapes::Sphere sphere, Material material) {
    m_spheres.push_back(sphere);
    m_materials.push_back(material);
  }

  [[nodiscard]] std::optional<ScatteringRecord> Hit(const Ray &r, float t_min, float t_max) const;

private:
  std::vector<Shapes::Sphere> m_spheres;
  std::vector<Material> m_materials;
};
} // namespace RTIAW::Render

#endif
