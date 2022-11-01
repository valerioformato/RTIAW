#ifndef RTIAW_renderutils
#define RTIAW_renderutils

#include <glm/glm.hpp>

#include <random>

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

namespace RTIAW::Random {
// All functions adapted from glm source code.
// On Linux the rand() syscall has global lock that really hurts multithreading performances

template <typename T> glm::vec<2, T, glm::defaultp> diskRand(T Radius) {
  static thread_local std::mt19937 generator{std::random_device{}()};
  static thread_local std::uniform_real_distribution<T> unif{T(-Radius), T(Radius)};

  assert(Radius > static_cast<T>(0));

  glm::vec<2, T, glm::defaultp> Result(T(0));
  T LenRadius(T(0));

  do {
    Result.x = unif(generator);
    Result.y = unif(generator);
    // Result = linearRand(glm::vec<2, T, glm::defaultp>(-Radius), vec<2, T, defaultp>(Radius));
    LenRadius = glm::length(Result);
  } while (LenRadius > Radius);

  return Result;
}

template <typename T> GLM_FUNC_QUALIFIER glm::vec<3, T, glm::defaultp> sphericalRand(T Radius) {
  static thread_local std::mt19937 generator{std::random_device{}()};
  static thread_local std::uniform_real_distribution<T> unif1{T(0.0), T(6.283185307179586476925286766559)};
  static thread_local std::uniform_real_distribution<T> unif2{T(-1), T(1)};

  assert(Radius > static_cast<T>(0));

  T theta = unif1(generator);
  T phi = std::acos(unif2(generator));

  T x = std::sin(phi) * std::cos(theta);
  T y = std::sin(phi) * std::sin(theta);
  T z = std::cos(phi);

  return glm::vec<3, T, glm::defaultp>(x, y, z) * Radius;
}

} // namespace RTIAW::Random

namespace RTIAW::Render {
using vec3 = glm::vec3;
using point3 = glm::vec3;
using color = glm::vec3;
} // namespace RTIAW::Render

#endif
