#ifndef RTIAW_renderutils
#define RTIAW_renderutils

#include <glm/glm/glm.hpp>

namespace RTIAW::Utils {
// Constants
constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = 3.1415926535897932385f;

// Utility Functions
inline float DegToRad(float degrees) { return degrees * pi / 180.0f; }
} // namespace RTIAW::Utils

namespace glm {
template <typename T, qualifier Q> float polar(glm::vec<3, T, Q> vec) {
  return RTIAW::Utils::pi - vec.z / std::sqrt(glm::dot(vec, vec));
}
template <typename T, qualifier Q> float azimuth(glm::vec<3, T, Q> vec) { return std::atan2(vec.x, vec.y); }
} // namespace glm

namespace RTIAW::Render {
using vec3 = glm::vec3;
using point3 = glm::vec3;
using color = glm::vec3;
} // namespace RTIAW::Render

#endif
