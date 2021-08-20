//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/API/RendererAPI.hh"

namespace Lorr
{
    class Renderer2D
    {
    public:
        ~Renderer2D();

        void Init(PlatformWindow *pWindow);

        void EndFrame();

    private:
        uint32_t m_ResetFlags = 0;
    };

}  // namespace Lorr