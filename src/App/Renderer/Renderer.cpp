#include <glm/gtx/string_cast.hpp>

#include "Renderer.h"

#include "Ray.h"

namespace RTIAW::Render {
void Renderer::SetImageSize(unsigned int x, unsigned int y) {
  m_imageSize = glm::uvec2{x, y};
  InitViewport();
}

void Renderer::Render(uint8_t *buffer) {
  m_logger->debug("Start rendering!!!");

  // helper vectors, as defined in the book :)
  const vec3 horizontal{m_viewportWidth, 0, 0};
  const vec3 vertical{0, m_viewportHeight, 0};
  auto lower_left_corner = m_cameraOrigin - horizontal / 2.0f - vertical / 2.0f - vec3{0, 0, m_cameraFocalLength};

  // temporary, for testing
  auto ray_color = [](const Ray &r) {
    const vec3 unit_direction = glm::normalize(r.direction());
    const auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
  };

  auto write_color = [buffer, this](unsigned int ix, unsigned int iy, const color &pixel_color) {
    // flip the vertical coordinate because the display backend follow the opposite convention
    iy = m_imageSize.y - 1 - iy;

    const unsigned int idx = 4 * (ix + iy * m_imageSize.x);
    buffer[idx] = static_cast<uint8_t>(255 * pixel_color.r);
    buffer[idx + 1] = static_cast<uint8_t>(255 * pixel_color.g);
    buffer[idx + 2] = static_cast<uint8_t>(255 * pixel_color.b);
    buffer[idx + 3] = 255;
  };

  for (int j = m_imageSize.y - 1; j >= 0; --j) {
    for (int i = 0; i < m_imageSize.x; ++i) {
      const auto u = static_cast<float>(i) / (m_imageSize.x - 1);
      const auto v = static_cast<float>(j) / (m_imageSize.y - 1);
      Ray r(m_cameraOrigin, lower_left_corner + u * horizontal + v * vertical - m_cameraOrigin);
      color pixel_color = ray_color(r);
      write_color(i, j, pixel_color);
    }
  }
}

} // namespace RTIAW::Render
