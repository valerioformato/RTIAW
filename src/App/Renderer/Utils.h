#ifndef RTIAW_renderutils
#define RTIAW_renderutils

#include <glm/glm.hpp>

namespace RTIAW::Utils {
// Constants
constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = 3.1415926535897932385f;

// Utility Functions
inline float DegToRad(float degrees) { return degrees * pi / 180.0f; }
} // namespace RTIAW::Utils

namespace RTIAW::Render {
using vec3 = glm::vec3;
using point3 = glm::vec3;
using color = glm::vec3;
} // namespace RTIAW::Render

#endif
