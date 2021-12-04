#ifndef RTIAW_materials_materials
#define RTIAW_materials_materials

#include <variant>

#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"

namespace RTIAW::Render {
using Material = std::variant<Materials::Lambertian, Materials::Dielectric, Materials::Metal>;
} // namespace RTIAW::Render

#endif
