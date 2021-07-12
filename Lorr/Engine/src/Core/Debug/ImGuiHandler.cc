#include "ImGuiHandler.hh"

#include "Engine.hh"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

namespace Lorr
{
    ImGuiHandler::~ImGuiHandler()
    {
    }

    void ImGuiHandler::Init( Engine *pEngine )
    {
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplDX11_Init( pEngine->GetAPI()->GetDevice(), pEngine->GetAPI()->GetDeviceContext() );
        ImGui_ImplWin32_Init( pEngine->GetWindow()->GetHandle() );
    }

    void ImGuiHandler::BeginFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiHandler::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
    }
}  // namespace Lorr