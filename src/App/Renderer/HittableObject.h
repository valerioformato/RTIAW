#ifndef RTIAW_hittableobject
#define RTIAW_hittableobject

#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {
struct DielectricSphere {
  DielectricSphere(Shapes::Sphere sphere, Materials::Dielectric material) : m_sphere(sphere), m_material(material) {}

  Shapes::Sphere m_sphere;
  Materials::Dielectric m_material;
};

struct LambertianSphere {
  LambertianSphere(Shapes::Sphere sphere, Materials::Lambertian material) : m_sphere(sphere), m_material(material) {}

  Shapes::Sphere m_sphere;
  Materials::Lambertian m_material;
};

struct MetalSphere {
  MetalSphere(Shapes::Sphere sphere, Materials::Metal material) : m_sphere(sphere), m_material(material) {}

  Shapes::Sphere m_sphere;
  Materials::Metal m_material;
};
} // namespace RTIAW::Render

#endif
