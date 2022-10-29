#include <functional>
#include <stdexcept>

// #include <tchar.h>

#include <imgui_internal.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Walnut/Image.h>

#include "ApplicationLayer.h"

namespace RTIAW {
ApplicationLayer::ApplicationLayer()
    : m_logger{spdlog::stdout_color_st("ApplicationLayer")}, m_renderer{std::make_unique<Render::Renderer>()} {
  // FIXME: remove after debugging
  spdlog::set_level(spdlog::level::debug);

  // m_renderer->SetImageSize(static_cast<unsigned int>(m_windowSize.x), static_cast<unsigned int>(m_windowSize.y));
  // m_renderer->SetTargetBuffer(m_imageBuffer.get());

  // testing!
  // TODO: later on let this be picked in a ImGui dropdown maybe?
  m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::TestScene);
  // m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::ThreeSpheres);
  // m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::DefaultScene);
}

/*
 void ApplicationLayer::Run() {
  m_logger->debug("Run window size: {} x {}", m_windowSize.x, m_windowSize.y);

  // Main loop
  bool done = false;
  while (!done) {
*/
void ApplicationLayer::OnUIRender() {
  // my stuff here?
  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::Begin("Rendered Image", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus);
  uint32_t imx = ImGui::GetContentRegionAvail().x;
  uint32_t imy = ImGui::GetContentRegionAvail().y;
  const void *imagebuffer = m_renderer->ImageBuffer();
  if (imagebuffer) {
    Walnut::Image image{imx, imy, Walnut::ImageFormat::RGBA, imagebuffer};
    ImGui::Image(image.GetDescriptorSet(), {(float)image.GetWidth(), (float)image.GetHeight()}, ImVec2(0, 1),
                 ImVec2(1, 0));
  }
  ImGui::End();
  ImGui::PopStyleVar(2);

  ImGui::SetNextWindowSize(ImVec2{150, 200});
  ImGui::Begin("Controls");
  // TODO: change this if/when we'll support multiple scenes
  ImGui::Text(fmt::format("Scene: {}", magic_enum::enum_name(m_renderer->Scene())).data());
  ImGui::Text(magic_enum::enum_name(m_renderer->State()).data());
  const bool startDisable = m_renderer->State() == Render::Renderer::RenderState::Running;
  if (startDisable) {
    ImGui::BeginDisabled();
  } else {
    // TODO: find out how to disable window resizing
    // EnableWindowResize();
  }
  if (ImGui::Button("Start!")) {
    // (re-)initialize buffer
    m_renderer->SetImageSize(imx, imy);
    // m_renderer->StartRender();
  }
  if (startDisable) {
    ImGui::EndDisabled();
    if (ImGui::Button("Abort")) {
      m_renderer->StopRender();
    }
  }
  ImGui::End();
}
} // namespace RTIAW
