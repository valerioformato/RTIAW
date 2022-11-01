#include <functional>
#include <stdexcept>

// #include <tchar.h>

#include <imgui_internal.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Walnut/Image.h>

#include "ApplicationLayer.h"

namespace RTIAW {
ApplicationLayer::ApplicationLayer() : m_logger{spdlog::stdout_color_st("ApplicationLayer")} {
  // FIXME: remove after debugging
  // spdlog::set_level(spdlog::level::debug);

  // TODO: later on let this be picked in a ImGui dropdown maybe?
  // m_renderer.SetScene(RTIAW::Render::Renderer::Scenes::TestScene);
  // m_renderer.SetScene(RTIAW::Render::Renderer::Scenes::ThreeSpheres);
  m_renderer.SetScene(RTIAW::Render::Renderer::Scenes::DefaultScene);
}

void ApplicationLayer::OnUIRender() {
  // my stuff here?
  //  ImGuiViewport *viewport = ImGui::GetMainViewport();
  //  ImGui::SetNextWindowPos(viewport->Pos);
  //  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::Begin("Rendered Image", nullptr, ImGuiWindowFlags_NoInputs);
  uint32_t imx = ImGui::GetContentRegionAvail().x;
  uint32_t imy = ImGui::GetContentRegionAvail().y;
  if (!m_image) {
    m_image = std::make_unique<Walnut::Image>(imx, imy, Walnut::ImageFormat::RGBA, nullptr);
  } else if (imx != m_image->GetWidth() || imy != m_image->GetHeight()) {
    m_image->Resize(imx, imy);
  }

  const void *imagebuffer = m_renderer.ImageBuffer();
  if (imagebuffer) {
    m_image->SetData(imagebuffer);
    ImGui::Image(m_image->GetDescriptorSet(), {(float)m_image->GetWidth(), (float)m_image->GetHeight()}, ImVec2(0, 1),
                 ImVec2(1, 0));
  }
  ImGui::End();
  ImGui::PopStyleVar(2);

  ImGui::Begin("Controls");
  // TODO: change this if/when we'll support multiple scenes
  // ImGui::Text(fmt::format("Scene: {}", magic_enum::enum_name(m_renderer.Scene())).data());
  // static std::underlying_type_t<Render::Renderer::Scenes> selected_scene_index = 0;
  if (ImGui::BeginCombo("Scene", magic_enum::enum_name(m_selectedScene).data())) {
    for (auto scene : magic_enum::enum_values<Render::Renderer::Scenes>()) {
      const bool is_selected = (m_selectedScene == scene);
      if (ImGui::Selectable(magic_enum::enum_name(scene).data(), is_selected))
        m_selectedScene = scene;

      // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImGui::Text(magic_enum::enum_name(m_renderer.State()).data());
  const bool startDisable = m_renderer.State() == Render::Renderer::RenderState::Running;
  if (startDisable) {
    ImGui::BeginDisabled();
  } else {
    // TODO: find out how to disable window resizing
    // EnableWindowResize();
  }
  if (ImGui::Button("Start!")) {
    // (re-)initialize buffer
    m_renderer.SetScene(m_selectedScene);
    m_renderer.SetImageSize(imx, imy);
    m_renderer.StartRender();
  }
  if (startDisable) {
    ImGui::EndDisabled();
    if (ImGui::Button("Abort")) {
      m_renderer.StopRender();
    }
  }
  ImGui::End();
}
} // namespace RTIAW
