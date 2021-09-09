//
// Created on July 10th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include <imgui.h>

namespace Lorr
{
    class Engine;
    class ImGuiHandler
    {
    public:
        ~ImGuiHandler();

        void Init(Engine *pEngine);

        void BeginFrame();
        void EndFrame();

    private:
        void InitImGui(PlatformWindow *pWindow);
        void ImGui_ImplSurface_Shutdown();
        void ImGui_ImplSurface_NewFrame();
    };
}  // namespace Lorr