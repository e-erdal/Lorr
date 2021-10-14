//
// Created on Wednesday 13th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/InputLayout.hh"
#include "Engine/Graphics/Common/ITexture.hh"

#include <dxgiformat.h>

namespace Lorr::D3D
{
    inline DXGI_FORMAT TextureFormatToDXFormat(uint32_t format)
    {
        switch (format)
        {
            case TEXTURE_FORMAT_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case TEXTURE_FORMAT_DEPTH_32: return DXGI_FORMAT_D32_FLOAT;
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
            default: return DXGI_FORMAT_UNKNOWN;
        }
    }

}  // namespace Lorr::D3D