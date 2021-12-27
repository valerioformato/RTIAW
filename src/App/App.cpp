#include <functional>
#include <stdexcept>
#include <tchar.h>

#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "App.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace RTIAW {
App::App(const ImVec2 pos, const ImVec2 windowSize)
    : m_windowSize{windowSize}, m_logger{spdlog::stdout_color_st("App")}, m_renderer{
                                                                              std::make_unique<Render::Renderer>()} {
  // FIXME: remove after debugging
  spdlog::set_level(spdlog::level::debug);

  // FIXME: FOR NOW ONLY WIN32... but this is outside the scope of the project :)
  // NOTE: taken from Imgui examples

  // Win32/D3D setup:
  wc = {sizeof(WNDCLASSEX),
        CS_CLASSDC,
        &App::WndMsgHandler,
        0L,
        0L,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        _T("ImGui Example"),
        nullptr};

  // Create application window
  // ImGui_ImplWin32_EnableDpiAwareness();
  ::RegisterClassEx(&wc);
  m_hwnd = ::CreateWindow(wc.lpszClassName, _T("Raytracing in a weekend!"), WS_OVERLAPPEDWINDOW,
                          static_cast<UINT>(pos.x), static_cast<UINT>(pos.y), static_cast<UINT>(windowSize.x),
                          static_cast<UINT>(windowSize.y), nullptr, nullptr, wc.hInstance, this);

  // setup our image buffer
  CreateImageBuffer();

  // and our main display window
  SetupMainWindow();

  m_renderer->SetImageSize(static_cast<unsigned int>(m_windowSize.x), static_cast<unsigned int>(m_windowSize.y));
  m_renderer->SetTargetBuffer(m_imageBuffer.get());

  // testing!
  // TODO: later on let this be picked in a ImGui dropdown maybe?
  m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::TestScene);
  // m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::ThreeSpheres);
  // m_renderer->SetScene(RTIAW::Render::Renderer::Scenes::DefaultScene);
}

App::~App() {
  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(m_hwnd);
  ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void App::Run() {
  auto *image_srv = CreateShaderBuffer();

  m_logger->debug("Run window size: {} x {}", m_windowSize.x, m_windowSize.y);

  // Main loop
  bool done = false;
  while (!done) {
    // Poll and handle messages (inputs, window resize, etc.)
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (m_resized) {
        image_srv = CreateShaderBuffer();
        m_resized = false;
      }
      if (msg.message == WM_QUIT) {
        done = true;
      }
    }
    if (done)
      break;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // my stuff here?
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
    UpdateTexture2D(image_srv);
    ImGui::Image(static_cast<void *>(image_srv), m_windowSize);
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
      EnableWindowResize();
    }
    if (ImGui::Button("Start!")) {
      // (re-)initialize buffer
      memset(m_imageBuffer.get(), 0, BufferSize());
      DisableWindowResize();
      m_renderer->StartRender();
    }
    if (startDisable) {
      ImGui::EndDisabled();
      if (ImGui::Button("Abort")) {
        m_renderer->StopRender();
      }
    }
    ImGui::End();

    // Rendering
    ImGui::Render();
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_pSwapChain->Present(1, 0); // Present with vsync
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

  D3D_FEATURE_LEVEL featureLevel;
  constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };

  if (constexpr UINT createDeviceFlags = 0;
      D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2,
                                    D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel,
                                    &m_pd3dDeviceContext) != S_OK) {
    return false;
  }

  CreateRenderTarget();
  return true;
}

void App::CleanupDeviceD3D() {
  CleanupRenderTarget();
  if (m_pSwapChain) {
    m_pSwapChain->Release();
    m_pSwapChain = nullptr;
  }
  if (m_pd3dDeviceContext) {
    m_pd3dDeviceContext->Release();
    m_pd3dDeviceContext = nullptr;
  }
  if (m_pd3dDevice) {
    m_pd3dDevice->Release();
    m_pd3dDevice = nullptr;
  }
}

void App::CreateRenderTarget() {
  ID3D11Texture2D *pBackBuffer = nullptr;
  m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  if (!pBackBuffer) {
    throw std::runtime_error("Could not create Imgui render buffer");
  }
  m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
  pBackBuffer->Release();
}

void App::CleanupRenderTarget() {
  if (m_mainRenderTargetView) {
    m_mainRenderTargetView->Release();
    m_mainRenderTargetView = nullptr;
  }
}

void App::ResizeWindow(const ImVec2 newSize) {
  m_windowSize = newSize;
  CreateImageBuffer();
  m_renderer->SetImageSize(static_cast<unsigned int>(m_windowSize.x), static_cast<unsigned int>(m_windowSize.y));
  m_renderer->SetTargetBuffer(m_imageBuffer.get());
}

void App::CreateImageBuffer() {
  auto newBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[BufferSize()]());
  m_imageBuffer.swap(newBuffer);
}

void App::SetupMainWindow() {
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

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);
}

ID3D11ShaderResourceView *App::CreateShaderBuffer() const {
  // Create texture
  D3D11_TEXTURE2D_DESC desc{};
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = static_cast<UINT>(m_windowSize.x);
  desc.Height = static_cast<UINT>(m_windowSize.y);
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  ID3D11Texture2D *pTexture = nullptr;
  D3D11_SUBRESOURCE_DATA subResource{};
  subResource.pSysMem = m_imageBuffer.get();
  subResource.SysMemPitch = desc.Width * 4;
  subResource.SysMemSlicePitch = 0;
  m_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
  if (!pTexture) {
    throw std::runtime_error("Could not initialize image buffer texture");
  }

  ID3D11ShaderResourceView *out_srv;

  // Create texture view
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  m_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
  pTexture->Release();

  return out_srv;
}

void App::UpdateTexture2D(ID3D11ShaderResourceView *srv) const {
  ID3D11Resource *res;
  srv->GetResource(&res);
  D3D11_MAPPED_SUBRESOURCE mres;
  m_pd3dDeviceContext->Map(res, 0, D3D11_MAP_WRITE_DISCARD, 0, &mres);

  if (const auto width = static_cast<unsigned int>(m_windowSize.x); mres.RowPitch == width * 4) {
    memcpy(mres.pData, m_imageBuffer.get(), BufferSize() * sizeof(uint8_t));
  } else {
    uint8_t *src = m_imageBuffer.get(), *dest = reinterpret_cast<uint8_t *>(mres.pData);
    for (unsigned int iy = 0; iy < m_windowSize.y; ++iy) {
      memcpy(dest, src, width * 4 * sizeof(uint8_t));
      dest += mres.RowPitch;
      src += width * 4;
    }
  }

  m_pd3dDeviceContext->Unmap(res, 0);
}

void App::EnableWindowResize() {
  if (!m_hwndResizeDisabled)
    return;
  SetWindowLong(m_hwnd, GWL_STYLE, m_hwndDefaultStyle);
  m_hwndResizeDisabled = false;
}

void App::DisableWindowResize() {
  m_hwndDefaultStyle = GetWindowLong(m_hwnd, GWL_STYLE);
  SetWindowLong(m_hwnd, GWL_STYLE, m_hwndDefaultStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
  m_hwndResizeDisabled = true;
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

    if (pThis->m_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
      const ImVec2 newSize{static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam))};
      pThis->CleanupRenderTarget();
      pThis->m_pSwapChain->ResizeBuffers(0, static_cast<UINT>(newSize.x), static_cast<UINT>(newSize.y),
                                         DXGI_FORMAT_UNKNOWN, 0);
      pThis->CreateRenderTarget();
      pThis->ResizeWindow(newSize);
      pThis->m_resized = true;
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  default:
    break;
  }

  return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
} // namespace RTIAW
