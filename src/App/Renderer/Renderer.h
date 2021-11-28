#ifndef RTIAW_renderer
#define RTIAW_renderer

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Utils.h"

namespace RTIAW::Render {

class Renderer {
public:
  Renderer() : m_logger{spdlog::stdout_color_st("Renderer")} {}

  void SetImageSize(unsigned int x, unsigned int y);

  void Render(uint8_t *buffer);

private:
  std::shared_ptr<spdlog::logger> m_logger;

  glm::uvec2 m_imageSize;

  // viewport properties
  float m_viewportHeight{2.0};
  float m_viewportWidth;

  float m_cameraFocalLength{1.0};
  point3 m_cameraOrigin{0, 0, 0};

  [[nodiscard]] float AspectRatio() const {
    return static_cast<float>(m_imageSize.x) / static_cast<float>(m_imageSize.y);
  }

  void InitViewport() { m_viewportWidth = m_viewportHeight * AspectRatio(); }
};
} // namespace RTIAW::Render

#endif
