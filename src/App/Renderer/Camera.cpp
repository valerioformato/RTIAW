#include <fmt/format.h>
#include <glm/gtc/random.hpp>

#include <random>

#include "Camera.h"

namespace RTIAW::Render {
Camera::Camera(CameraOrientation orientation, float verticalFov, float aspectRatio, float aperture, float focusDist) {
  auto theta = Utils::DegToRad(verticalFov);
  auto h = std::tan(theta / 2);
  m_viewportHeight = 2.0f * h;
  m_viewportWidth = aspectRatio * m_viewportHeight;
  m_lensRadius = aperture / 2.0f;

  w = glm::normalize(orientation.lookfrom - orientation.lookat);
  u = glm::normalize(glm::cross(orientation.vup, w));
  v = glm::cross(w, u);

  m_origin = orientation.lookfrom;
  m_horizontal = focusDist * m_viewportWidth * u;
  m_vertical = focusDist * m_viewportHeight * v;
  m_lowerLeftCorner = m_origin - m_horizontal / 2.0f - m_vertical / 2.0f - focusDist * w;
}

Ray Camera::NewRay(float s, float t) const {
  glm::vec2 randVec = Random::diskRand(m_lensRadius);

  vec3 offset = u * randVec.x + v * randVec.y;
  float ray_time = Random::uniformRand(m_shutterOpen, m_shutterClose);

  return Ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset, ray_time);
}
} // namespace RTIAW::Render
