#ifndef RTIAW_window
#define RTIAW_window

#include <d3d11.h>
#include <fmt/format.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "Renderer/Renderer.h"

namespace RTIAW {
class App {
public:
  App(const ImVec2 pos, const ImVec2 windowSize);
  ~App();

  void Run();

private:
  ImVec2 m_windowSize;
  std::shared_ptr<spdlog::logger> m_logger;

  // here we store our internal representation of the rendered image
  std::unique_ptr<uint8_t[]> m_imageBuffer;

  // this is our big boy, who'll do all the work
  std::unique_ptr<Render::Renderer> m_renderer;

  [[nodiscard]] size_t BufferSize() const { return static_cast<size_t>(m_windowSize.x * m_windowSize.y) * 4; }
  void ResizeWindow(const ImVec2 newSize);
  void CreateImageBuffer();

  void SetupMainWindow();
  [[nodiscard]] ID3D11ShaderResourceView *DrawImageBuffer() const;
  void UpdateTexture2D(ID3D11ShaderResourceView *srv) const;

  // below here only boilerplate... :)

  // win32 stuff
  HWND m_hwnd;
  WNDCLASSEX wc; // literally no idea how win32 API works :\

  // window message handler
  static LRESULT WINAPI WndMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  // D3D backend stuff
  ID3D11Device *m_pd3dDevice = nullptr;
  ID3D11DeviceContext *m_pd3dDeviceContext = nullptr;
  IDXGISwapChain *m_pSwapChain = nullptr;
  ID3D11RenderTargetView *m_mainRenderTargetView = nullptr;

  // helper functions
  bool CreateDeviceD3D();
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
};
} // namespace RTIAW

#endif
