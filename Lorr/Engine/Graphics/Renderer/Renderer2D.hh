//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include "VertexBatcher.hh"

namespace Lorr
{
    class Renderer2D
    {
    public:
        ~Renderer2D();

        void Init(PlatformWindow *pWindow);

        void BeginFrame();
        void EndFrame();

        void SetVSyncState(bool VSync);

    private:
        void Reset();

    private:
        uint32_t m_ResetFlags = 0;
        uint32_t m_ResWidth = 0;
        uint32_t m_ResHeight = 0;

        VertexBatcher m_Batcher;
    };

}  // namespace Lorr