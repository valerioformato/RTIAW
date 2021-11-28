#include <glm/gtc/random.hpp>

#include "Camera.h"

namespace RTIAW::Render {
Camera::Camera(CameraOrientation orientation, float verticalFov, float aspectRatio, float aperture, float focusDist) {
  auto theta = Utils::DegToRad(verticalFov);
  auto h = std::tan(theta / 2);
  m_viewportHeight = 2.0 * h;
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
  glm::vec2 randVec = glm::diskRand(m_lensRadius);
  vec3 offset = u * randVec.x + v * randVec.y;

  return Ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset);
}
} // namespace RTIAW::Render
