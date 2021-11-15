//
// Created on August 8th 2021 by e-erdal.
// D3D11 like low level texture wrapper
//

#pragma once

#include "Engine/Resource/IResource.hh"

#define TEXTURE_MAG_NEAREST (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_UVW_CLAMP)

namespace Lorr
{
    enum TextureFormat : u16
    {
        TEXTURE_FORMAT_RGBA8,
        TEXTURE_FORMAT_RGBAF32,
        TEXTURE_FORMAT_RGBF32,
        TEXTURE_FORMAT_DEPTH_32F,
        TEXTURE_FORMAT_DEPTH_D24UNS8U,
        TEXTURE_FORMAT_R24G8_TYPELESS,
        TEXTURE_FORMAT_R32_TYPELESS,
        TEXTURE_FORMAT_R32_FLOAT
    };

    constexpr u32 TextureFormatToSize(TextureFormat format)
    {
        switch (format)
        {
            case TEXTURE_FORMAT_RGBA8: return sizeof(float);
            case TEXTURE_FORMAT_RGBAF32: return sizeof(float) * 4;
            case TEXTURE_FORMAT_RGBF32: return sizeof(float) * 3;
            case TEXTURE_FORMAT_DEPTH_32F: return sizeof(float);
            case TEXTURE_FORMAT_DEPTH_D24UNS8U: return sizeof(u32);
            case TEXTURE_FORMAT_R24G8_TYPELESS: return sizeof(u32);
            case TEXTURE_FORMAT_R32_TYPELESS: return sizeof(u32);
            case TEXTURE_FORMAT_R32_FLOAT: return sizeof(float);
            default: return 0;
        }
    }

    enum TextureType : u8
    {
        TEXTURE_TYPE_REGULAR,
        TEXTURE_TYPE_DEPTH,
        TEXTURE_TYPE_RENDER_TARGET,
        TEXTURE_TYPE_RW,
        TEXTURE_TYPE_STAGING
    };

    struct TextureDesc
    {
        TextureType Type = TEXTURE_TYPE_REGULAR;
        u32 Filters = 0;
        u32 MipMapLevels = 1;
    };

    struct TextureData
    {
        u32 Width = 0;
        u32 Height = 0;

        TextureFormat Format = TEXTURE_FORMAT_RGBA8;

        u32 DataSize = 0;
        u8 *Data = 0;

        ~TextureData()
        {
            SAFE_FREE(Data);
        }
    };

    class ITexture : public IResource<TextureDesc, TextureData>
    {
    public:
        virtual void Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData) = 0;
        virtual void Use() = 0;
        virtual void Delete() = 0;
        virtual void *GetHandle() = 0;

        virtual void GenerateMips() = 0;

        static void ParseToMemory(TextureData *pOutData, BufferStream &imageBuffer);
        static constexpr ResourceType m_ResType = ResourceType::Texture;

        virtual ~ITexture(){};

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

        u32 m_UsingMip = 0;

    protected:
        u32 m_Width = 0;
        u32 m_Height = 0;
        u32 m_DataSize = 0;
        u32 m_Filters = 0;
        u32 m_TotalMips = 1;

        TextureFormat m_Format;
    };

    typedef ITexture *TextureHandle;

    namespace Texture
    {
        TextureHandle Create(const Identifier &ident, const std::string &path, TextureDesc *pDesc);
        TextureHandle Create(const Identifier &ident, TextureDesc *pDesc, TextureData *pData);
    }  // namespace Texture
}  // namespace Lorr