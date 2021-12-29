//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseRenderBuffer.hh"

namespace lr
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

        ShaderProgram *CreateProgram(const Identifier &ident, const InputLayout &layout, const eastl::string &vertexPath,
                                     const eastl::string &pixelPath);
        ShaderProgram *GetProgram(const Identifier &ident);

        ShaderHandle CreateShader(const Identifier &ident, const InputLayout &layout, const eastl::string &vertexPath);  // Vertex shader
        ShaderHandle CreateShader(const Identifier &ident, const eastl::string &path);                                   // Other shaders
        ShaderHandle GetShader(const Identifier &ident);

        RenderBufferHandle CreateRenderBuffer(const Identifier &ident, const RenderBufferDesc &desc);
        RenderBufferHandle GetRenderBuffer(const Identifier &ident);

    private:
        eastl::unordered_map<Identifier, ShaderHandle> m_Shaders;
        eastl::unordered_map<Identifier, ShaderProgram> m_Programs;
        eastl::unordered_map<Identifier, RenderBufferHandle> m_Buffers;  // TODO: Move this to BufferManager
    };
}  // namespace lr