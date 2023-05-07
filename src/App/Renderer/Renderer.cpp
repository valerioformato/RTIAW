#include <execution>
#include <optional>
#include <ranges>

#include <fmt/chrono.h>
#include <glm/gtc/random.hpp>

#include "Ray.h"
#include "Renderer.h"

// #include "Materials/Material.h"
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

void Renderer::SetImageSize(unsigned int x, unsigned int y) {
  m_imageSize = glm::uvec2{x, y};

  // reset all buffers
  m_renderBuffer.clear();
  m_renderBuffer.resize(x * y * 4);
  m_renderBufferAccumulator.clear();
  m_renderBufferAccumulator.resize(x * y * 4);
  m_renderBufferSamples.clear();
  m_renderBufferSamples.resize(x * y);
}

void Renderer::StartRender() {
  switch (m_state) {
  case RenderState::Finished:
  case RenderState::Stopped:
    m_renderingThread.join();
    break;
  default:
    break;
  }

  LoadScene();
  m_renderingThread = std::thread{&Renderer::Render, this};
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

void Renderer::Render() {
  m_logger->debug("Start rendering!!!");
  const auto startTime = std::chrono::system_clock::now();

  m_state = RenderState::Running;

  auto renderLine = [this](const unsigned int lineCoord) {
    if (m_state == RenderState::Stopped) {
      return;
    }

    for (unsigned int i = 0; i < m_imageSize.x; ++i) {
      const auto pixelCoord = glm::uvec2{i, lineCoord};
      color pixel_color{0, 0, 0};
      for (unsigned int i_sample = 0; i_sample < samplesPerPixel; ++i_sample) {
        const auto u = (static_cast<float>(pixelCoord.x) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.x - 1);
        const auto v = (static_cast<float>(pixelCoord.y) + m_unifDistribution(m_rnGenerator)) / (m_imageSize.y - 1);
        Ray r = m_camera->NewRay(u, v);
        pixel_color += ShootRay(r, maxRayDepth);
      }
      WritePixelToBuffer(pixelCoord.x, pixelCoord.y, samplesPerPixel, pixel_color);
    }
  };

  auto renderQuad = [this](const glm::uvec2 minCoo, const glm::uvec2 maxCoo) {
    if (m_state == RenderState::Stopped) {
      return;
    }

    std::mt19937 generator{std::random_device{}()};

    for (unsigned int j = maxCoo.y; j > minCoo.y; --j) {
      for (unsigned int i = minCoo.x; i < maxCoo.x; ++i) {
        color pixel_color{0, 0, 0};
        const auto pixelCoord = glm::uvec2{i, j - 1};
        for (unsigned int i_sample = 0; i_sample < samplesPerPixel; ++i_sample) {
          const auto u = (static_cast<float>(pixelCoord.x) + m_unifDistribution(generator)) / (m_imageSize.x - 1);
          const auto v = (static_cast<float>(pixelCoord.y) + m_unifDistribution(generator)) / (m_imageSize.y - 1);
          Ray r = m_camera->NewRay(u, v);
          pixel_color += ShootRay(r, maxRayDepth);
        }
        WritePixelToBuffer(pixelCoord.x, pixelCoord.y, samplesPerPixel, pixel_color);
      }
    }
  };

  auto renderPixel = [this](unsigned int x, unsigned int y) {
    if (m_state == RenderState::Stopped) {
      return;
    }

    std::mt19937 generator{std::random_device{}()};

    const auto u = (x + m_unifDistribution(generator)) / (m_imageSize.x - 1);
    const auto v = (y + m_unifDistribution(generator)) / (m_imageSize.y - 1);
    Ray r = m_camera->NewRay(u, v);
    color pixel_color = ShootRay(r, maxRayDepth);
    WritePixelToBuffer(x, y, 1, pixel_color);
  };

  if (shouldAccumulate) {
    std::vector<glm::uvec2> pixels(m_imageSize.x * m_imageSize.y);
    for (size_t pix_idx = 0; pix_idx < pixels.size(); ++pix_idx) {
      pixels[pix_idx].y = pix_idx % m_imageSize.y;
      pixels[pix_idx].x = m_imageSize.x - pix_idx / m_imageSize.y - 1;
    }

    for (unsigned int isample = 0; isample < samplesPerPixel; ++isample) {
      std::for_each(std::execution::par_unseq, begin(pixels), end(pixels),
                    [renderPixel](glm::uvec2 pixel) { renderPixel(pixel.x, pixel.y); });
    }
  } else {
    switch (m_strategy) {
    case Strategy::Lines: { // Render per-line
      auto rlines = std::ranges::views::iota(0) | std::ranges::views::take(m_imageSize.y) | std::ranges::views::reverse;
      std::vector<unsigned int> lines(rlines.begin(), rlines.end());
      std::for_each(std::execution::par_unseq, begin(lines), end(lines),
                    [renderLine](unsigned int line_idx) { renderLine(line_idx); });
      break;
    }
    case Strategy::Quads: { // Render per-quad
      auto quads = SplitImage();
      std::for_each(std::execution::par_unseq, begin(quads), end(quads),
                    [renderQuad](const Quad &quad) { renderQuad(quad.minCoo, quad.maxCoo); });
      break;
    }
    }
  }

  const auto stopTime = std::chrono::system_clock::now();
  auto renderDuration = stopTime - startTime;
  m_logger->debug("Rendering took {}", std::chrono::duration_cast<std::chrono::seconds>(renderDuration));
  lastRenderTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(renderDuration).count();

  m_state = RenderState::Finished;
}

color Renderer::ShootRay(const Ray &ray, unsigned int depth) {
  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth == 0)
    return color(0, 0, 0);

  if (const auto &[o_hitRecord, o_scatterResult] = m_scene.Hit(ray, 0.001f, RTIAW::Utils::infinity); o_hitRecord) {
    const auto &[t, p, normal, matIdx, front_face] = o_hitRecord.value();

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

void Renderer::WritePixelToBuffer(unsigned int ix, unsigned int iy, unsigned int samples_per_pixel, color pixel_color) {
  const unsigned int img_idx = 4 * (ix + iy * m_imageSize.x);
  m_renderBufferAccumulator[img_idx] += pixel_color.r;
  m_renderBufferAccumulator[img_idx + 1] += pixel_color.g;
  m_renderBufferAccumulator[img_idx + 2] += pixel_color.b;
  m_renderBufferAccumulator[img_idx + 3] += 1;

  const unsigned int sample_idx = ix + iy * m_imageSize.x;
  m_renderBufferSamples[sample_idx] += samples_per_pixel;
}

const void *Renderer::ImageBuffer() {
  if (m_renderBuffer.empty())
    return nullptr;
  else {
    RenderImage();
    return m_renderBuffer.data();
  }
}

void Renderer::RenderImage() {
  for (unsigned int pix_idx = 0; pix_idx < m_renderBufferSamples.size(); ++pix_idx) {
    color pixel_color{m_renderBufferAccumulator[4 * pix_idx], m_renderBufferAccumulator[4 * pix_idx + 1],
                      m_renderBufferAccumulator[4 * pix_idx + 2]};
    pixel_color /= m_renderBufferSamples[pix_idx];
    pixel_color = glm::sqrt(pixel_color);
    pixel_color = glm::clamp(pixel_color, 0.0f, 1.0f);

    m_renderBuffer[4 * pix_idx] = static_cast<uint8_t>(255 * pixel_color.r);
    m_renderBuffer[4 * pix_idx + 1] = static_cast<uint8_t>(255 * pixel_color.g);
    m_renderBuffer[4 * pix_idx + 2] = static_cast<uint8_t>(255 * pixel_color.b);
    m_renderBuffer[4 * pix_idx + 3] = 255;
  }
};

} // namespace RTIAW::Render
