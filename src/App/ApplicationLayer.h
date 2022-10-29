#ifndef RTIAW_window
#define RTIAW_window

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <Walnut/Layer.h>

#include "Renderer/Renderer.h"

namespace RTIAW {
class ApplicationLayer : public Walnut::Layer {
public:
  ApplicationLayer();
  ~ApplicationLayer() override = default;

  void OnUIRender() override;

  void Run();

private:
  std::shared_ptr<spdlog::logger> m_logger;

  // this is our big boy, who'll do all the work
  std::unique_ptr<Render::Renderer> m_renderer;
};
} // namespace RTIAW

#endif
