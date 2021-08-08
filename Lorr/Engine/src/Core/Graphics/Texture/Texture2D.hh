//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Utils/Identifier.hh"

#include <bgfx/bgfx.h>

#define TEXTURE_FORMAT_NEAREST ( BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT )

namespace Lorr
{
    class Texture2D
    {
    private:
        Identifier m_Ident = EmptyIdentifier;

        uint32_t m_iWidth = 0;
        uint32_t m_iHeight = 0;

        uint32_t m_uDataSize = 0;

        bgfx::TextureHandle m_thHandle = BGFX_INVALID_HANDLE;
    };
}  // namespace Lorr