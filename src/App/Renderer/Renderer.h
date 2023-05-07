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
  enum class Strategy { Quads, Lines };
  enum class Scenes { DefaultScene, ThreeSpheres, TestScene };

  Renderer() : m_logger{spdlog::stdout_color_st("Renderer")} {}
  Renderer(const Renderer &) = delete;
  ~Renderer();

  void SetImageSize(unsigned int x, unsigned int y);
  void SetScene(Scenes scene = Scenes::DefaultScene) { m_sceneType = scene; };
  void SetStrategy(Strategy strat = Strategy::Quads) { m_strategy = strat; };

  void SetSamplesPerPixel(unsigned int nSamples) { samplesPerPixel = nSamples; }
  void SetMaxRayBounces(unsigned int nBounces) { maxRayDepth = nBounces; }

  void StartRender();
  void StopRender();

  [[nodiscard]] Scenes Scene() const { return m_sceneType; }
  [[nodiscard]] RenderState State() const { return m_state; }
  [[nodiscard]] const void *ImageBuffer();

  unsigned int samplesPerPixel = 64;
  unsigned int maxRayDepth = 12;
  unsigned int lastRenderTimeMS = 0;
  bool shouldAccumulate = false;

private:
  std::shared_ptr<spdlog::logger> m_logger;

  glm::uvec2 m_imageSize{0, 0};

  Scenes m_sceneType{Scenes::DefaultScene};
  Strategy m_strategy{Strategy::Quads};
  HittableObjectList m_scene;
  void LoadScene();

  RenderState m_state = RenderState::Ready;

  // render buffer
  std::vector<uint8_t> m_renderBuffer{};
  std::vector<float> m_renderBufferAccumulator{};
  std::vector<float> m_renderBufferSamples{};

  // main rendering thread
  std::thread m_renderingThread;
  Utils::Pool m_threadPool{8};

  // our camera :)
  std::unique_ptr<Camera> m_camera;

  struct Quad {
    Quad(glm::uvec2 min, glm::uvec2 max) : minCoo{min}, maxCoo{max} {};
    glm::uvec2 minCoo;
    glm::uvec2 maxCoo;
  };
  std::vector<Quad> SplitImage(unsigned int quadSize = 100) const;
  // actual internal implementation
  void Render();
  void RenderImage();
  color ShootRay(const Ray &ray, unsigned int depth);
  void WritePixelToBuffer(unsigned int ix, unsigned int iy, unsigned int samples_per_pixel, color pixel_color);

  // rng stuff
  std::mt19937 m_rnGenerator{};
  std::uniform_real_distribution<float> m_unifDistribution{0.0f, 1.0f};

  [[nodiscard]] float AspectRatio() const {
    return static_cast<float>(m_imageSize.x) / static_cast<float>(m_imageSize.y);
  }
};
} // namespace RTIAW::Render

#endif
