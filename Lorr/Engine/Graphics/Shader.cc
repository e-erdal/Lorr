#include "Shader.hh"

#define FIND_SHADER(type)                                                                                                                                                \
    std::find_if(pData->Shaders.begin(), pData->Shaders.end(), [](const ShaderInfo &x) { return x.Renderer == bgfx::getRendererType() && x.Type == ShaderType::type; })

namespace Lorr
{
    void Shader::Init(const Identifier &ident, ShaderData *pData, bool isCompute)
    {
        m_Ident = ident;

        if (!isCompute)
        {
            auto vertex = FIND_SHADER(Vertex);
            auto frag = FIND_SHADER(Fragment);
            if (vertex == pData->Shaders.end() && frag == pData->Shaders.end()) return;

            const auto *vertBuffer = bgfx::copy(vertex->pData, vertex->Len);
            const auto vertexHandle = bgfx::createShader(vertBuffer);

            const auto *fragBuffer = bgfx::copy(frag->pData, frag->Len);
            const auto fragmentHandle = bgfx::createShader(fragBuffer);

            m_Handle = bgfx::createProgram(vertexHandle, fragmentHandle, true);
        }
    }

    void Shader::ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer)
    {
        // Runtime shader compilation is not supported
    }

}  // namespace Lorr