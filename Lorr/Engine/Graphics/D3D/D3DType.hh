//
// Created on Wednesday 13th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/Type.hh"

#include "Engine/Graphics/Common/InputLayout.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include <dxgiformat.h>
#include <d3dcommon.h>

namespace lr::D3D
{
    inline DXGI_FORMAT TextureFormatToDXFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case TextureFormat::RGBA16: return DXGI_FORMAT_R16G16B16A16_UNORM;
            case TextureFormat::RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;

            case TextureFormat::R24TG8T: return DXGI_FORMAT_R24G8_TYPELESS;

            case TextureFormat::R32T: return DXGI_FORMAT_R32_TYPELESS;
            case TextureFormat::R32U: return DXGI_FORMAT_R32_UINT;
            case TextureFormat::R32F: return DXGI_FORMAT_R32_FLOAT;

            case TextureFormat::DEPTH32F: return DXGI_FORMAT_D32_FLOAT;
            case TextureFormat::DEPTH24_STENCIL8: return DXGI_FORMAT_D24_UNORM_S8_UINT;

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
            case VertexAttribType::Vec3_3x10: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
            case VertexAttribType::Vec4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case VertexAttribType::UInt: return DXGI_FORMAT_R32_UINT;
            default: return DXGI_FORMAT_UNKNOWN;
        }
    }

    constexpr D3D_PRIMITIVE_TOPOLOGY ToDXPrimitive(PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::PointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            case PrimitiveType::LineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            case PrimitiveType::LineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
            case PrimitiveType::TriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case PrimitiveType::TriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        }
    }

}  // namespace lr::D3D