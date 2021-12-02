//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseRenderBuffer.hh"

namespace Lorr
{
    struct ShaderProgram
    {
        ShaderHandle Vertex = 0;
        ShaderHandle Pixel;
    };

    class ShaderManager
    {
    public:
        ShaderManager() = default;

        void Init();

        ShaderProgram *CreateProgram(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath, std::string_view pixelPath);
        ShaderProgram *GetProgram(const Identifier &ident);

        ShaderHandle CreateShader(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath);  // Vertex shader
        ShaderHandle CreateShader(const Identifier &ident, std::string_view path);                                   // Other shaders
        ShaderHandle GetShader(const Identifier &ident);

        RenderBufferHandle CreateRenderBuffer(const Identifier &ident, const RenderBufferDesc &desc);
        RenderBufferHandle GetRenderBuffer(const Identifier &ident);

    private:
        std::unordered_map<Identifier, ShaderHandle> m_Shaders;
        std::unordered_map<Identifier, ShaderProgram> m_Programs;
        std::unordered_map<Identifier, RenderBufferHandle> m_Buffers;  // TODO: Move this to BufferManager
    };
}  // namespace Lorr