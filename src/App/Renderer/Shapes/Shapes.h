#ifndef RTIAW_shapes_shapes
#define RTIAW_shapes_shapes

#include <variant>

#include "App/Renderer/Shapes/Parallelogram.h"
#include "App/Renderer/Shapes/Plane.h"
#include "App/Renderer/Shapes/Rectangle.h"
#include "App/Renderer/Shapes/Sphere.h"

namespace RTIAW::Render {
using Shape = std::variant<Shapes::Sphere, Shapes::Plane, Shapes::Parallelogram, Shapes::Rectangle>;
} // namespace RTIAW::Render

#endif
