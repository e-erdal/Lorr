#include "ShaderManager.hh"

namespace lr
{
    void ShaderManager::Init()
    {
    }

    ShaderProgram *ShaderManager::CreateProgram(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath, std::string_view pixelPath)
    {
        ZoneScoped;
        
        ShaderProgram program;

        ShaderDesc vertexDesc;
        vertexDesc.Layout = layout;
        program.Vertex = Shader::Create(fmt::format("{}-v", ident), vertexPath.data(), &vertexDesc);
        program.Pixel = Shader::Create(fmt::format("{}-p", ident), pixelPath.data());

        if (program.Vertex && program.Pixel)
        {
            return &m_Programs.emplace(ident, program).first->second;  // ?
        }

        return nullptr;
    }

    ShaderProgram *ShaderManager::GetProgram(const Identifier &ident)
    {
        ZoneScoped;
        
        const auto &it = m_Programs.find(ident);
        if (it == m_Programs.end()) return 0;
        return &it->second;
    }

    ShaderHandle ShaderManager::CreateShader(const Identifier &ident, const InputLayout &layout, std::string_view vertexPath)
    {
        ZoneScoped;
        
        ShaderDesc vertexDesc;
        vertexDesc.Layout = layout;
        if (ShaderHandle shader = Shader::Create(fmt::format("{}", ident), vertexPath.data(), &vertexDesc))
        {
            return m_Shaders.emplace(ident, shader).first->second;
        }

        return nullptr;
    }

    ShaderHandle ShaderManager::CreateShader(const Identifier &ident, std::string_view path)
    {
        ZoneScoped;
        
        if (ShaderHandle shader = Shader::Create(fmt::format("{}", ident), path.data()))
        {
            return m_Shaders.emplace(ident, shader).first->second;
        }

        return nullptr;
    }

    ShaderHandle ShaderManager::GetShader(const Identifier &ident)
    {
        ZoneScoped;
        
        const auto &it = m_Shaders.find(ident);
        if (it == m_Shaders.end()) return 0;
        return it->second;
    }

    RenderBufferHandle ShaderManager::CreateRenderBuffer(const Identifier &ident, const RenderBufferDesc &desc)
    {
        ZoneScoped;
        
        RenderBufferHandle buffer = RenderBuffer::Create(desc);
        if (buffer) m_Buffers.emplace(ident, buffer);
        return buffer;  // no need for null check
    }

    RenderBufferHandle ShaderManager::GetRenderBuffer(const Identifier &ident)
    {
        ZoneScoped;
        
        const auto &it = m_Buffers.find(ident);
        if (it == m_Buffers.end()) return 0;
        return it->second;
    }

}  // namespace lr
