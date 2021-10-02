//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "Engine/Resource/IResource.hh"

const std::unordered_map<std::string, bgfx::RendererType::Enum> kShaderRenderers = {
    { "glc", bgfx::RendererType::OpenGL },    { "met", bgfx::RendererType::Metal },      { "spv", bgfx::RendererType::Vulkan },
    { "vs3", bgfx::RendererType::Direct3D9 }, { "vs4", bgfx::RendererType::Direct3D11 }, { "vs5", bgfx::RendererType::Direct3D12 },
};

namespace Lorr
{
    enum class ShaderType : uint8_t
    {
        Vertex,
        Fragment,
        Compute
    };

    struct ShaderInfo
    {
        bgfx::RendererType::Enum Renderer;
        ShaderType Type;
        uint32_t Len;
        uint8_t *pData;
    };

    struct ShaderData
    {
        uint8_t TypeCount = 0;
        std::vector<ShaderInfo> Shaders;
    };

    class Shader : public IResource
    {
    public:
        void Init(const Identifier &ident, ShaderData *pData, bool isCompute = false);
        static void ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer);

        static constexpr ResourceType m_Type = ResourceType::Shader;

    public:
        const Identifier &GetIdentifier() const
        {
            return m_Ident;
        }

        const auto &GetHandle() const
        {
            return m_Handle;
        }

    protected:
        Identifier m_Ident = EmptyIdentifier;

        bgfx::ProgramHandle m_Handle;
    };

}  // namespace Lorr
