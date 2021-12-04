#ifndef RTIAW_shapes_shapes
#define RTIAW_shapes_shapes

#include <variant>

#include "Shapes/Sphere.h"

namespace RTIAW::Render {
using Shape = std::variant<Shapes::Sphere>;
} // namespace RTIAW::Render

#endif
