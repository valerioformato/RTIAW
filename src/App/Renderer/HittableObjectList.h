#ifndef RTIAW_hittableobjectlist
#define RTIAW_hittableobjectlist

#include <memory>
#include <optional>
#include <vector>

#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {

class HittableObjectList {
public:
  HittableObjectList() = default;

  inline void Clear() {
    m_dielectric_spheres.clear();
    m_lambertian_spheres.clear();
    m_metal_spheres.clear();
    m_dielectric_materials.clear();
    m_lambertian_materials.clear();
    m_metal_materials.clear();
  }

  inline void Add(Shapes::Sphere sphere, Materials::Dielectric material) {
    m_dielectric_spheres.push_back(sphere);
    m_dielectric_materials.push_back(material);
  }
  inline void Add(Shapes::Sphere sphere, Materials::Lambertian material) {
    m_lambertian_spheres.push_back(sphere);
    m_lambertian_materials.push_back(material);
  }
  inline void Add(Shapes::Sphere sphere, Materials::Metal material) {
    m_metal_spheres.push_back(sphere);
    m_metal_materials.push_back(material);
  }
  [[nodiscard]] std::optional<ScatteringRecord> Hit(const Ray &r, float t_min, float t_max) const;

private:
  std::vector<Shapes::Sphere> m_dielectric_spheres;
  std::vector<Shapes::Sphere> m_lambertian_spheres;
  std::vector<Shapes::Sphere> m_metal_spheres;
  std::vector<Materials::Dielectric> m_dielectric_materials;
  std::vector<Materials::Lambertian> m_lambertian_materials;
  std::vector<Materials::Metal> m_metal_materials;
};
} // namespace RTIAW::Render

#endif
