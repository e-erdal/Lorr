//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Utils/Identifier.hh"

#include <bgfx/bgfx.h>

#define TEXTURE_FORMAT_NEAREST ( BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT )

namespace Lorr
{
    struct TEXTURE2D_DESC
    {
        uintptr_t Filters = 0;
        std::string Path = "";
    };

    struct TEXTURE2D_DESC_SUBRESC
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        bgfx::TextureFormat::Enum Format = bgfx::TextureFormat::RGBA8;

        uint8_t *Data = 0;
        size_t DataSize = 0;
    };

    class Texture2D
    {
    public:
        void Init( Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES );

    private:
        Identifier m_Ident = "empty::";

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        uint32_t m_DataSize = 0;

        bgfx::TextureHandle m_Handle = BGFX_INVALID_HANDLE;
    };
}  // namespace Lorr