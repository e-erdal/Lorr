//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Core/Window/PlatformWindow.hh"

#include <bgfx/bgfx.h>

namespace Lorr
{
    class Renderer2D
    {
    public:
        ~Renderer2D();

        void Init( PlatformWindow *pWindow );
    };

}  // namespace Lorr