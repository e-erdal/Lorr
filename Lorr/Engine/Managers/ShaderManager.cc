#include "ShaderManager.hh"

namespace Lorr
{
    void ShaderManager::Init()
    {
    }

    void ShaderManager::CreateProgram(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath, std::string_view pixelPath)
    {
        ShaderProgram program;

        ShaderDesc vertexDesc;
        vertexDesc.Layout = layout;
        program.Vertex = Shader::Create(fmt::format("{}-v", ident), vertexPath.data(), &vertexDesc);
        program.Pixel = Shader::Create(fmt::format("{}-p", ident), pixelPath.data());

        if (program.Vertex && program.Pixel)
        {
            m_Programs.emplace(ident, program);
        }
    }

    ShaderProgram *ShaderManager::Get(const Identifier &ident)
    {
        const auto &it = m_Programs.find(ident);

        if (it == m_Programs.end()) return 0;

        return &it->second;
    }

    void ShaderManager::Use(const Identifier &ident)
    {
    }

    RenderBufferHandle ShaderManager::GetCBuf(const Identifier &ident)
    {
        const auto &it = m_CBuffers.find(ident);

        if (it == m_CBuffers.end()) return 0;

        return it->second;
    }

}  // namespace Lorr
