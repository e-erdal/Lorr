//
// Created on Wednesday 13th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/InputLayout.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include <dxgiformat.h>

namespace Lorr::D3D
{
    enum class DepthFunc : u8
    {
        Never = 1,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    enum class Cull : u8
    {
        None = 1,
        Front,
        Back,
    };

    inline DXGI_FORMAT TextureFormatToDXFormat(u32 format)
    {
        switch (format)
        {
            case TEXTURE_FORMAT_RGBA8U: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case TEXTURE_FORMAT_RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;

            case TEXTURE_FORMAT_RGB32F: return DXGI_FORMAT_R32G32B32_FLOAT;

            case TEXTURE_FORMAT_R24TG8T: return DXGI_FORMAT_R24G8_TYPELESS;

            case TEXTURE_FORMAT_R32T: return DXGI_FORMAT_R32_TYPELESS;
            case TEXTURE_FORMAT_R32F: return DXGI_FORMAT_R32_FLOAT;

            case TEXTURE_FORMAT_DEPTH32F: return DXGI_FORMAT_D32_FLOAT;
            case TEXTURE_FORMAT_DEPTH24_STENCIL8: return DXGI_FORMAT_D24_UNORM_S8_UINT;

            default: return DXGI_FORMAT_UNKNOWN;
        }
    }

    inline DXGI_FORMAT VertexTypeToDXFormat(VertexAttribType type)
    {
        switch (type)
        {
            case VertexAttribType::Float: return DXGI_FORMAT_R32_FLOAT;
            case VertexAttribType::Vec2: return DXGI_FORMAT_R32G32_FLOAT;
            case VertexAttribType::Vec3: return DXGI_FORMAT_R32G32B32_FLOAT;
            case VertexAttribType::Vec4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case VertexAttribType::UInt: return DXGI_FORMAT_R8G8B8A8_UNORM;
            default: return DXGI_FORMAT_UNKNOWN;
        }
    }

}  // namespace Lorr::D3D