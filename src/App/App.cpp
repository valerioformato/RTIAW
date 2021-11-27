#include <functional>
#include <stdexcept>
#include <tchar.h>

#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>

#include "App.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace RTIAW {
App::App(ImVec2 pos, ImVec2 size) {
  // FIXME: FOR NOW ONLY WIN32... but this is outside the scope of the project :)
  // NOTE: taken from Imgui examples

  // Win32/D3D setup:
  wc = {sizeof(WNDCLASSEX),  CS_CLASSDC, &App::WndMsgHandler, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        _T("ImGui Example"), NULL};

  // Create application window
  // ImGui_ImplWin32_EnableDpiAwareness();
  ::RegisterClassEx(&wc);
  m_hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280,
                          800, NULL, NULL, wc.hInstance, this);

  // horrible hack around win32 API

  // Initialize Direct3D
  if (!CreateDeviceD3D()) {
    CleanupDeviceD3D();
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    throw std::runtime_error("Cannot initialize Direct3D device");
  }

  // Show the window
  ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(m_hwnd);

  // Imgui setup:
  //
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

App::~App() {
  m_mainThread.join();

  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(m_hwnd);
  ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void App::Run() {
  // Main loop
  bool done = false;
  while (!done) {
    // Poll and handle messages (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
    // inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two
    // flags.
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        done = true;
    }
    if (done)
      break;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(nullptr);

    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = {m_defaultBkgColor.x * m_defaultBkgColor.w,
                                             m_defaultBkgColor.y * m_defaultBkgColor.w,
                                             m_defaultBkgColor.z * m_defaultBkgColor.w, m_defaultBkgColor.w};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // Present with vsync
    // g_pSwapChain->Present(0, 0); // Present without vsync
  }
}

// Helper functions

bool App::CreateDeviceD3D() {
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = m_hwnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[2] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };
  if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2,
                                    D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel,
                                    &g_pd3dDeviceContext) != S_OK)
    return false;

  CreateRenderTarget();
  return true;
}

void App::CleanupDeviceD3D() {
  CleanupRenderTarget();
  if (g_pSwapChain) {
    g_pSwapChain->Release();
    g_pSwapChain = NULL;
  }
  if (g_pd3dDeviceContext) {
    g_pd3dDeviceContext->Release();
    g_pd3dDeviceContext = NULL;
  }
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = NULL;
  }
}

void App::CreateRenderTarget() {
  ID3D11Texture2D *pBackBuffer;
  g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
  pBackBuffer->Release();
}

void App::CleanupRenderTarget() {
  if (g_mainRenderTargetView) {
    g_mainRenderTargetView->Release();
    g_mainRenderTargetView = NULL;
  }
}

// Win32 message handler
LRESULT WINAPI App::WndMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (::ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

  App *pThis = nullptr;

  switch (msg) {
  case WM_NCCREATE:
    pThis = static_cast<App *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);
    SetLastError(0);
    if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
      if (GetLastError() != 0)
        return false;
    }
    break;
  case WM_SIZE:
    pThis = reinterpret_cast<App *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (pThis->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
      pThis->CleanupRenderTarget();
      pThis->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
      pThis->CreateRenderTarget();
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  }
  return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
} // namespace RTIAW
