#include <optional>

#include <fmt/chrono.h>
#include <glm/gtc/random.hpp>

#include "Ray.h"
#include "Renderer.h"

#include "App/App.h"
//#include "Materials/Material.h"
#include "HittableObjectList.h"

#include <numeric>

namespace RTIAW::Render {
Renderer::~Renderer() {
  switch (m_state) {
  case RenderState::Finished:
  case RenderState::Stopped:
    m_renderingThread.join();
    break;
  default:
    break;
  }
}

void Renderer::SetImageSize(unsigned int x, unsigned int y) { m_imageSize = glm::uvec2{x, y}; }

void Renderer::StartRender() {
  switch (m_state) {
  case RenderState::Finished:
  case RenderState::Stopped:
    m_renderingThread.join();
    break;
  default:
    break;
  }

  m_renderingThread = std::thread{&Renderer::Render, this, m_renderBuffer};
}

void Renderer::StopRender() { m_state = RenderState::Stopped; }

std::vector<Renderer::Quad> Renderer::SplitImage(unsigned int quadSize) const {
  std::vector<Quad> result;
  const unsigned int nX = static_cast<unsigned int>(std::ceil(m_imageSize.x / static_cast<float>(quadSize)));
  const unsigned int nY = static_cast<unsigned int>(std::ceil(m_imageSize.y / static_cast<float>(quadSize)));
  for (unsigned int j = nY; j > 0; --j) {
    for (unsigned int i = 0; i < nX; ++i) {
      result.emplace_back(glm::uvec2{i * quadSize, (j - 1) * quadSize},
                          glm::min(glm::uvec2{(i + 1) * quadSize, j * quadSize}, m_imageSize));
    }
  }

  return result;
}

void Renderer::Render(uint8_t *buffer) {
  m_logger->debug("Start rendering!!!");
  const auto startTime = std::chrono::system_clock::now();

  m_state = RenderState::Running;

  auto renderLine = [this, buffer](const unsigned int lineCoord) {
    if (m_state == RenderState::Stopped) {
      return;
    }

    for (unsigned int i = 0; i < m_imageSize.x; ++i) {
      const auto pixelCoord = glm::uvec2{i, lineCoord};
      color pixel_color{0, 0, 0};
      for (unsigned int i_sample = 0; i_sample < m_samplesPerPixel; ++i_sample) {
        const auto u = (static_cast<float>(pixelCoord.x) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.x - 1);
        const auto v = (static_cast<float>(pixelCoord.y) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.y - 1);
        Ray r = m_camera->NewRay(u, v);
        pixel_color += ShootRay(r, m_maxRayDepth);
      }
      WritePixelToBuffer(buffer, pixelCoord.x, pixelCoord.y, m_samplesPerPixel, pixel_color);
    }
  };

  auto renderQuad = [this, buffer](const glm::uvec2 minCoo, const glm::uvec2 maxCoo) {
    if (m_state == RenderState::Stopped) {
      return;
    }

    for (unsigned int j = maxCoo.y; j > minCoo.y; --j) {
      for (unsigned int i = minCoo.x; i < maxCoo.x; ++i) {
        color pixel_color{0, 0, 0};
        const auto pixelCoord = glm::uvec2{i, j - 1};
        for (unsigned int i_sample = 0; i_sample < m_samplesPerPixel; ++i_sample) {
          const auto u = (static_cast<float>(pixelCoord.x) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.x - 1);
          const auto v = (static_cast<float>(pixelCoord.y) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.y - 1);
          Ray r = m_camera->NewRay(u, v);
          pixel_color += ShootRay(r, m_maxRayDepth);
        }
        WritePixelToBuffer(buffer, pixelCoord.x, pixelCoord.y, m_samplesPerPixel, pixel_color);
      }
    }
  };

  std::vector<std::future<void>> futures;

  // Render per-line
  // for (int j = m_imageSize.y - 1; j >= 0; --j) {
  //  futures.push_back(m_threadPool.AddTask(renderLine, j));
  //}

  // Render per-quad
  for (const auto &[minCoo, maxCoo] : SplitImage()) {
    futures.push_back(m_threadPool.AddTask(renderQuad, minCoo, maxCoo));
  }

  // wait until all tasks are done...
  std::for_each(begin(futures), end(futures), [](auto &future) { future.wait(); });

  auto stopTime = std::chrono::system_clock::now();
  m_logger->debug("Rendering took {}", std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime));

  m_state = RenderState::Finished;
}

color Renderer::ShootRay(const Ray &ray, unsigned int depth) {
  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth == 0)
    return color(0, 0, 0);

  if (const auto &[o_hitRecord, o_scatterResult] = m_scene.Hit(ray, 0.001f, RTIAW::Utils::infinity); o_hitRecord) {
    const auto &[p, normal, t, front_face] = o_hitRecord.value();

    if (o_scatterResult) {
      const auto &[attenuation, scattered] = o_scatterResult.value();
      return attenuation * ShootRay(scattered, depth - 1);
    }

    return {0, 0, 0};
  }

  constexpr color white{1.0, 1.0, 1.0};
  constexpr color azure{0.5, 0.7, 1.0};

  float t = 0.5f * (ray.direction.y + 1.0f);
  return (1.0f - t) * white + t * azure;
}

void Renderer::WritePixelToBuffer(uint8_t *buffer, unsigned int ix, unsigned int iy, unsigned int samples_per_pixel,
                                  color pixel_color) const {
  // flip the vertical coordinate because the display backend follow the opposite convention
  iy = m_imageSize.y - 1 - iy;

  pixel_color /= samples_per_pixel;
  pixel_color = glm::sqrt(pixel_color);
  pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);

  const unsigned int idx = 4 * (ix + iy * m_imageSize.x);
  buffer[idx] = static_cast<uint8_t>(255 * pixel_color.r);
  buffer[idx + 1] = static_cast<uint8_t>(255 * pixel_color.g);
  buffer[idx + 2] = static_cast<uint8_t>(255 * pixel_color.b);
  buffer[idx + 3] = 255;
};

} // namespace RTIAW::Render
