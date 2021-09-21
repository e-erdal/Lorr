//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

namespace Lorr
{
    enum class ShaderType : uint8_t
    {
        Vertex,
        Fragment,
        Compute
    };

    struct FileShader
    {
        bgfx::RendererType::Enum Renderer;
        ShaderType Type;
        uint32_t Len;
        uint8_t *pData;
    };

}  // namespace Lorr
