//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    struct ShaderProgram
    {
        ShaderHandle Vertex = 0;
        ShaderHandle Pixel;
        ShaderHandle Compute;
    };

    class ShaderManager
    {
    public:
        ShaderManager() = default;

        void Init();

        void CreateProgram(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath, std::string_view pixelPath);
        ShaderProgram *Get(const Identifier &ident);
        void Use(const Identifier &ident);

        template<typename T>
        void CreateCBuffer(const Identifier &ident, T &t, bool dynamic = false)
        {
            RenderBufferUsage usage = dynamic ? RenderBufferUsage::Dynamic : RenderBufferUsage::Default;
            RenderBufferAccess access = dynamic ? RB_ACCESS_TYPE_CPUW : RB_ACCESS_TYPE_NONE;
            RenderBufferHandle buffer = RenderBuffer::Create(&t, sizeof(T), RenderBufferType::Constant, usage, access);

            if (buffer) m_CBuffers.emplace(ident, buffer);
        }

        RenderBufferHandle GetCBuf(const Identifier &ident);

    private:
        std::unordered_map<Identifier, ShaderProgram> m_Programs;
        std::unordered_map<Identifier, RenderBufferHandle> m_CBuffers;  // TODO: Move this to BufferManager
    };
}  // namespace Lorr