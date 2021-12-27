#ifndef RTIAW_renderer
#define RTIAW_renderer

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Camera.h"
#include "HittableObjectList.h"
#include "ThreadPool.h"
#include "Utils.h"

#include <random>

namespace RTIAW::Render {

class Renderer {
public:
  enum class RenderState { Ready, Running, Finished, Stopped };
  enum class Scenes { DefaultScene, ThreeSpheres, TestScene };

  Renderer() : m_logger{spdlog::stdout_color_st("Renderer")} {}
  Renderer(const Renderer &) = delete;
  ~Renderer();

  void SetImageSize(unsigned int x, unsigned int y);
  void SetTargetBuffer(uint8_t *const buffer) { m_renderBuffer = buffer; }
  void SetScene(Scenes scene = Scenes::DefaultScene);

  void SetSamplesPerPixel(unsigned int nSamples) { m_samplesPerPixel = nSamples; }
  void SetMaxRayBounces(unsigned int nBounces) { m_maxRayDepth = nBounces; }

  void StartRender();
  void StopRender();

  [[nodiscard]] Scenes Scene() const { return m_sceneType; }
  [[nodiscard]] RenderState State() const { return m_state; }

private:
  std::shared_ptr<spdlog::logger> m_logger;

  glm::uvec2 m_imageSize{0, 0};
  unsigned int m_samplesPerPixel = 32;
  unsigned int m_maxRayDepth = 10;

  Scenes m_sceneType{Scenes::DefaultScene};
  HittableObjectList m_scene;
  RenderState m_state = RenderState::Ready;

  // render buffer
  uint8_t *m_renderBuffer = nullptr;

  // main rendering thread
  std::thread m_renderingThread;
  Utils::Pool m_threadPool{};

  // our camera :)
  std::unique_ptr<Camera> m_camera;

  struct Quad {
    Quad(glm::uvec2 min, glm::uvec2 max) : minCoo{min}, maxCoo{max} {};
    glm::uvec2 minCoo;
    glm::uvec2 maxCoo;
  };
  std::vector<Quad> SplitImage(unsigned int quadSize = 100) const;
  // actual internal implementation
  void Render(uint8_t *buffer);
  color ShootRay(const Ray &ray, unsigned int depth);
  void WritePixelToBuffer(uint8_t *buffer, unsigned int ix, unsigned int iy, unsigned int samples_per_pixel,
                          color pixel_color) const;

  // rng stuff
  std::mt19937 m_rnGenerator{};
  std::uniform_real_distribution<float> m_unifDistribution{0.0f, 1.0f};

  [[nodiscard]] float AspectRatio() const {
    return static_cast<float>(m_imageSize.x) / static_cast<float>(m_imageSize.y);
  }
};
} // namespace RTIAW::Render

#endif
