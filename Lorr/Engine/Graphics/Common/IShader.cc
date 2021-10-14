#include "IShader.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Graphics/D3D/D3DProfile.hh"
#include "Engine/Graphics/Common/IRenderer.hh"
#include "Engine/Graphics/D3D11/D3D11Shader.hh"

#include <d3dcompiler.h>
#include <d3d11shader.h>

#define FIND_SHADER(type)                                                                                                                  \
    std::find_if(pData->Shaders.begin(), pData->Shaders.end(),                                                                             \
                 [](const ShaderInfo &x) { return x.Renderer == Lorr::RendererAPI::CurrentAPI() && x.Type == ShaderType::type; })

namespace Lorr
{
    void IShader::ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer)
    {
        HRESULT hr;

        std::string shader((const char *)inBuffer.GetData(), inBuffer.GetSize());
        ShaderType currentType;
        if (shader.substr(0, 9) == "//$TYPE v")
        {
            currentType = ShaderType::Vertex;
        }
        else if (shader.substr(0, 9) == "//$TYPE p")
        {
            currentType = ShaderType::Pixel;
        }

        constexpr uint32_t flags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY | D3DCOMPILE_OPTIMIZATION_LEVEL3;
        ID3DBlob *pCode = 0;
        ID3DBlob *pError = 0;

        // clang-format off
        if ((hr = 
            D3DCompile(inBuffer.GetData(), inBuffer.GetSize(), "LorrShader", 0, 0, "main", 
            D3D::GetLatestShaderFeature(currentType).data(), flags, 0, &pCode, &pError)) < 0)
        {
            LOG_ERROR("Failed to compile shader. -- {} -- {}", (char *)pError->GetBufferPointer(), D3D::GetLatestShaderFeature(currentType));
            return;
        }
        // clang-format on

        pOutData->Type = currentType;
        pOutData->Renderer = APIType::D3D11;
        pOutData->Buffer.Reset((uint8_t *)pCode->GetBufferPointer(), pCode->GetBufferSize());

        SAFE_RELEASE(pCode);
        SAFE_RELEASE(pError);
    }

    ShaderHandle Shader::Create(const Identifier &ident, const std::string &path, ShaderDesc *pDesc)
    {
        switch (IRenderer::CurrentAPI())
        {
            case APIType::D3D11: return GetEngine()->GetResourceMan()->LoadResource<D3D11Shader>(ident, path, pDesc);
            default: break;
        }

        return 0;
    }

    ShaderHandle Shader::Create(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData)
    {
        switch (IRenderer::CurrentAPI())
        {
            case APIType::D3D11:
            {
                D3D11Shader *shader = new D3D11Shader;

                shader->Init(ident, pDesc, pData);

                return (ShaderHandle)shader;
            }
            default: break;
        }

        return 0;
    }

}  // namespace Lorr