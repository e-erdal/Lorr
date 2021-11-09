//
// Created on August 8th 2021 by e-erdal.
// D3D11 like low level texture wrapper
//

#pragma once

#include "Engine/Resource/IResource.hh"

#define TEXTURE_MAG_NEAREST (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_UVW_CLAMP)

namespace Lorr
{
    enum TextureFormat : uint16_t
    {
        TEXTURE_FORMAT_RGBA8,
        TEXTURE_FORMAT_RGBAF32,
        TEXTURE_FORMAT_DEPTH_32F,
        TEXTURE_FORMAT_DEPTH_D24UNS8U,
        TEXTURE_FORMAT_R24G8_TYPELESS,
        TEXTURE_FORMAT_R32_TYPELESS,
        TEXTURE_FORMAT_R32_FLOAT
    };

    enum TextureType : uint8_t
    {
        TEXTURE_TYPE_REGULAR,
        TEXTURE_TYPE_DEPTH,
        TEXTURE_TYPE_RENDER_TARGET,
        TEXTURE_TYPE_RW,
    };

    struct TextureDesc
    {
        TextureType Type = TEXTURE_TYPE_REGULAR;
        uint32_t Filters = 0;
        uint32_t MipMapLevels = 1;
    };

    struct TextureData
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        TextureFormat Format = TEXTURE_FORMAT_RGBA8;

        uint32_t DataSize = 0;
        uint8_t *Data = 0;
    };

    class ITexture : public IResource<TextureDesc, TextureData>
    {
    public:
        virtual void Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData) = 0;
        virtual void Use() = 0;
        virtual void Delete() = 0;
        virtual void *GetHandle() = 0;

        // Step by step functions
        virtual void CreateHandle(TextureDesc *pDesc, TextureData *pData) = 0;
        virtual void CreateShaderResource(TextureDesc *pDesc) = 0;
        virtual void CreateSampler(TextureDesc *pDesc) = 0;
        virtual void CreateUAV() = 0;

        virtual void GenerateMips() = 0;

        static void ParseToMemory(TextureData *pOutData, BufferStream &imageBuffer);
        static constexpr ResourceType m_ResType = ResourceType::Texture;

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

        uint32_t m_UsingMip = 0;

    protected:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_DataSize = 0;
        uint32_t m_Filters = 0;
    };

    typedef ITexture *TextureHandle;

    namespace Texture
    {
        TextureHandle Create(const Identifier &ident, const std::string &path, TextureDesc *pDesc);
        TextureHandle Create(const Identifier &ident, TextureDesc *pDesc, TextureData *pData);
    }  // namespace Texture
}  // namespace Lorr