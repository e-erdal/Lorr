//
// Created on August 8th 2021 by e-erdal.
//
// D3D11 like low level texture wrapper
//

#pragma once

#include "Engine/Resource/IResource.hh"

#define TEXTURE_MAG_NEAREST (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT)

namespace Lorr
{
    struct Texture2DData
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        bgfx::TextureFormat::Enum Format = bgfx::TextureFormat::RGBA8;

        uint32_t DataSize = 0;
        uint8_t *Data = 0;
    };

    class Texture2D : public IResource
    {
    public:
        void Init(const Identifier &ident, uint32_t Filters, Texture2DData *pData);
        static void ParseToMemory(Texture2DData *pOutData, BufferStream &imageBuffer);

        static constexpr ResourceType m_Type = ResourceType::Texture;

    public:
        const auto &GetWidth() const
        {
            return m_Width;
        }

        const auto &GetHeight() const
        {
            return m_Height;
        }

        const auto &GetDataSize() const
        {
            return m_DataSize;
        }

        const auto &GetFormat() const
        {
            return m_Format;
        }

    private:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_DataSize = 0;
        uint32_t m_Filters = 0;

        bgfx::TextureFormat::Enum m_Format = bgfx::TextureFormat::RGBA8;

        bgfx::TextureHandle m_Handle = BGFX_INVALID_HANDLE;
    };
}  // namespace Lorr