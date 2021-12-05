#include "BaseShader.hh"

#include "Engine/App/Engine.hh"

#include "BaseRenderer.hh"
#include "Engine/Graphics/D3D/D3DProfile.hh"
#include "Engine/Graphics/D3D11/D3D11Shader.hh"

#include <d3dcompiler.h>
#include <d3d11shader.h>

#define FIND_SHADER(type)                                                                                                                                      \
    std::find_if(pData->Shaders.begin(), pData->Shaders.end(), [](const ShaderInfo &x) {                                                                       \
        return x.Renderer == Lorr::IRenderer::CurrentAPI() && x.Type == ShaderType::type;                                                                      \
    })

namespace Lorr
{
    void BaseShader::ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer)
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
        else if (shader.substr(0, 9) == "//$TYPE c")
        {
            currentType = ShaderType::Compute;
        }

        constexpr u32 flags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY | D3DCOMPILE_OPTIMIZATION_LEVEL3;
        ID3DBlob *pCode = 0;
        ID3DBlob *pError = 0;

        // clang-format off
        if ((hr = 
            D3DCompile(inBuffer.GetData(), inBuffer.GetSize(), "main", nullptr, nullptr, "main", 
            D3D::GetLatestShaderFeature(currentType).data(), flags, 0, &pCode, &pError)) < 0)
        {
            LOG_ERROR("Failed to compile shader. -- {} -- {}", (char *)pError->GetBufferPointer(), D3D::GetLatestShaderFeature(currentType));
            return;
        }
        // clang-format on

        pOutData->Type = currentType;
        pOutData->Renderer = RendererType::D3D11;
        pOutData->Buffer.Reset((u8 *)pCode->GetBufferPointer(), pCode->GetBufferSize());

        SAFE_RELEASE(pCode);
        SAFE_RELEASE(pError);
    }

    ShaderHandle Shader::Create(const Identifier &ident, const std::string &path, ShaderDesc *pDesc)
    {
        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11: return GetEngine()->GetResourceMan()->LoadResource<D3D11Shader>(ident, path, pDesc);
            default: break;
        }

        return 0;
    }

    ShaderHandle Shader::Create(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData)
    {
        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11:
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