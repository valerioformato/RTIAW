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
    m_spheres.clear();
    m_metal_offset = m_lambertian_offset = 0;
    m_dielectric_materials.clear();
    m_metal_materials.clear();
    m_lambertian_materials.clear();
  }

  inline void Add(Shapes::Sphere sphere, Materials::Dielectric material) {
    m_spheres.insert(m_spheres.begin() + m_metal_offset, sphere);
    m_dielectric_materials.push_back(material);
    ++m_metal_offset;
    ++m_lambertian_offset;
  }
  inline void Add(Shapes::Sphere sphere, Materials::Metal material) {
    m_spheres.insert(m_spheres.begin() + m_lambertian_offset, sphere);
    m_metal_materials.push_back(material);
    ++m_lambertian_offset;
  }
  inline void Add(Shapes::Sphere sphere, Materials::Lambertian material) {
    m_spheres.push_back(sphere);
    m_lambertian_materials.push_back(material);
  }
  [[nodiscard]] std::optional<ScatteringRecord> Hit(const Ray &r, float t_min, float t_max) const;

private:
  // enum class MaterialType : char { DIELECTRIC, LAMBERTIAN, METAL };

  std::vector<Shapes::Sphere> m_spheres;
  size_t m_metal_offset = 0;
  size_t m_lambertian_offset = 0;
  std::vector<Materials::Dielectric> m_dielectric_materials;
  std::vector<Materials::Metal> m_metal_materials;
  std::vector<Materials::Lambertian> m_lambertian_materials;
};
} // namespace RTIAW::Render

#endif
