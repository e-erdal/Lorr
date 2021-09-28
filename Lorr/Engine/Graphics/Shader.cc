#include "Shader.hh"

namespace Lorr
{
    void Shader::Init(const Identifier &ident, ShaderData *pData, ShaderType type)
    {
        m_Ident = ident;

        auto found =
            std::find_if(pData->Shaders.begin(), pData->Shaders.end(), [type](const ShaderInfo &x) { return x.Renderer == bgfx::getRendererType() && x.Type == type; });

        if (found != pData->Shaders.end())
        {
            const auto *buffer = bgfx::copy(found->pData, found->Len);
            m_Handle = bgfx::createShader(buffer);
            bgfx::setName(m_Handle, ident);
        }
    }

    void Shader::ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer)
    {
        // Runtime shader compilation is not supported
    }

}  // namespace Lorr