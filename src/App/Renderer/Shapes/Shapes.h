#ifndef RTIAW_shapes_shapes
#define RTIAW_shapes_shapes

#include <variant>

#include "Shapes/Plane.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {
using Shape = std::variant<Shapes::Sphere, Shapes::Plane>;
} // namespace RTIAW::Render

#endif
