#ifndef RTIAW_window
#define RTIAW_window

#include <thread>

#include <d3d11.h>
#include <imgui/imgui.h>

namespace RTIAW {
class App {
public:
  App(ImVec2 pos, ImVec2 size);
  ~App();

  void Run();

private:
  std::thread m_mainThread;

  ImVec4 m_defaultBkgColor{0.0f, 0.0f, 0.0f, 0.0f};

  // win32 stuff
  HWND m_hwnd;
  WNDCLASSEX wc; // literally no idea how win32 API works :\

  // window message handler
  static LRESULT WINAPI WndMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  // D3D stuff
  ID3D11Device *g_pd3dDevice = nullptr;
  ID3D11DeviceContext *g_pd3dDeviceContext = nullptr;
  IDXGISwapChain *g_pSwapChain = nullptr;
  ID3D11RenderTargetView *g_mainRenderTargetView = nullptr;

  // helper functions
  bool CreateDeviceD3D();
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
};
} // namespace RTIAW

#endif
