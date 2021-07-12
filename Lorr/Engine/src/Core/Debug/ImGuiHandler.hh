//
// Created on July 10th 2021 by e-erdal.
//

#pragma once

#include <imgui.h>

namespace Lorr
{
    class Engine;
    class ImGuiHandler
    {
    public:
        ~ImGuiHandler();

        void Init( Engine *pEngine );

        void BeginFrame();
        void EndFrame();
    };
}  // namespace Lorr