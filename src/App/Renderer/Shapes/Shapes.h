#ifndef RTIAW_shapes_shapes
#define RTIAW_shapes_shapes

#include <variant>

#include "Shapes/Parallelogram.h"
#include "Shapes/Plane.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {
using Shape = std::variant<Shapes::Sphere, Shapes::Plane, Shapes::Parallelogram, Shapes::Rectangle>;
} // namespace RTIAW::Render

#endif
