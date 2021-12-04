#ifndef RTIAW_scatteringrecord
#define RTIAW_scatteringrecord

#include "Ray.h"
#include "Utils.h"

namespace RTIAW::Render {
struct ScatteringRecord {
  color attenuation{0, 0, 0};
  Ray ray{};
};
} // namespace RTIAW::Render

#endif
