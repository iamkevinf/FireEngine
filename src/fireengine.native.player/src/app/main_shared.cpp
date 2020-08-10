#include "main_shared.h"
#include "imgui.h"
#include "bgfx/bgfx.h"
#include "../imgui/imgui_impl_bgfx.h"

#include "bx/bx.h"

#ifdef BX_COMPILER_MSVC
bool ImGui_ImplWin32_Init(void* hwnd);
void ImGui_ImplWin32_NewFrame();
void ImGui_ImplWin32_Shutdown();
#endif

#ifdef __linux__
bool ImGui_ImplX11_Init(void* hwnd);
void ImGui_ImplX11_NewFrame();
void ImGui_ImplX11_Shutdown();
#endif

void PlatformInit(void* window, int w, int h)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    bgfx::Init init;
    init.type = bgfx::RendererType::Enum::Count;
    init.resolution.width = w;
    init.resolution.height = h;
    init.resolution.reset = BGFX_RESET_FLUSH_AFTER_RENDER;// BGFX_RESET_VSYNC;
    init.platformData.nwh = window;
    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    bgfx::frame();

    // Setup Platform/Renderer bindings
#ifdef BX_COMPILER_MSVC
    ImGui_ImplWin32_Init(window);
#endif
#ifdef __linux__
    ImGui_ImplX11_Init(window);
#endif
    ImGui_Implbgfx_Init();
}

void PlatformFrame(void(*OnGUI)())
{
    ImGuiIO& io = ImGui::GetIO();

    bgfx::touch(0);
    // Start the Dear ImGui frame
    ImGui_Implbgfx_NewFrame();
#ifdef BX_COMPILER_MSVC
    ImGui_ImplWin32_NewFrame();
#endif
#ifdef __linux__
    ImGui_ImplX11_NewFrame();
#endif
    ImGui::NewFrame();

    OnGUI();

    // Rendering
    ImGui::EndFrame();

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawData(ImGui::GetDrawData());
    bgfx::frame();

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void PlatformFinalize()
{
    ImGui_Implbgfx_Shutdown();
#ifdef BX_COMPILER_MSVC
    ImGui_ImplWin32_Shutdown();
#endif
#ifdef __linux__
    ImGui_ImplX11_Shutdown();
#endif
    ImGui::DestroyContext();

    // Shutdown bgfx.
    bgfx::shutdown();
}