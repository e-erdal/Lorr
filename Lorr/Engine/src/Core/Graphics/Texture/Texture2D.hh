//
// Created on August 8th 2021 by e-erdal.
//
// D3D11 like low level texture wrapper
//

#pragma once

#include "Utils/Identifier.hh"

namespace Lorr
{
    enum eTextureFormat
    {
        TEXTURE_FORMAT_RGB8,  // unsupported
        TEXTURE_FORMAT_RGBA8,
    };

    enum eTextureFiltering
    {
        TEXTURE_FILTER_NEAREST,
        TEXTURE_FILTER_LINEAR,
    };

    struct TEXTURE2D_DESC
    {
        eTextureFiltering Filters = TEXTURE_FILTER_LINEAR;
        std::string Path = "";
    };

    struct TEXTURE2D_DESC_SUBRESC
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        eTextureFormat Format = TEXTURE_FORMAT_RGBA8;

        uint8_t *Data = 0;
        size_t DataSize = 0;
    };

    class Texture2D
    {
    public:
        static Texture2D *Create(Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES = 0);

    protected:
        virtual void Init(Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES = 0) = 0;

        Identifier m_Ident = EmptyIdentifier;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        uint32_t m_DataSize = 0;

        // bgfx::TextureHandle m_Handle = BGFX_INVALID_HANDLE;
    };
}  // namespace Lorr