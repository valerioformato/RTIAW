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
  spdlog::set_level(spdlog::level::debug);

  m_renderer.SetScene(RTIAW::Render::Renderer::Scenes::DefaultScene);
}

void ApplicationLayer::OnUIRender() {
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
    m_renderer.SetStrategy(m_renderStrategy);
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

  ImGui::Begin("Render Settings");
  ImGui::DragInt("Samples", (int *)&m_renderer.samplesPerPixel, 1, 1, 1024);
  ImGui::DragInt("Bounces", (int *)&m_renderer.maxRayDepth, 1, 1, 1024);

  ImGui::Separator();

  ImGui::Checkbox("Continuous rendering", &m_renderer.shouldAccumulate);
  if (!m_renderer.shouldAccumulate) {
    if (ImGui::BeginCombo("Render strategy", magic_enum::enum_name(m_renderStrategy).data())) {
      for (auto strategy : magic_enum::enum_values<Render::Renderer::Strategy>()) {
        const bool is_selected = (m_renderStrategy == strategy);
        if (ImGui::Selectable(magic_enum::enum_name(strategy).data(), is_selected))
          m_renderStrategy = strategy;

        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  }

  ImGui::Separator();

  ImGui::Text("Last render time: %d ms", m_renderer.lastRenderTimeMS);

  ImGui::End();
}
} // namespace RTIAW
