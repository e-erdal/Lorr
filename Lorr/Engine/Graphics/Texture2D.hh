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
    struct TEXTURE2D_DESC : IRESOURCE_DESC
    {
        uint32_t Filters = 0;
        std::string Path = "";
    };

    struct TEXTURE2D_DESC_SUBRESC : IRESOURCE_DESC_SUBRESC
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        bgfx::TextureFormat::Enum Format = bgfx::TextureFormat::RGBA8;

        uint32_t DataSize = 0;
        uint8_t *Data = 0;
    };

    class Texture2D : public IResource<TEXTURE2D_DESC, TEXTURE2D_DESC_SUBRESC>
    {
    public:
        void Init(const Identifier &ident, TEXTURE2D_DESC *pDesc, TEXTURE2D_DESC_SUBRESC *pSubRes = 0) override;
        void ParseMemory(TEXTURE2D_DESC_SUBRESC *pSubResc, BufferStream &buffer) override;

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
        Identifier m_Ident = EmptyIdentifier;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_DataSize = 0;

        bgfx::TextureFormat::Enum m_Format = bgfx::TextureFormat::RGBA8;

        bgfx::TextureHandle m_Handle = BGFX_INVALID_HANDLE;
    };
}  // namespace Lorr