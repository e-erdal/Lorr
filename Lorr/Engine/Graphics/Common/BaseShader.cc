#include "BaseShader.hh"

#include "Engine/App/Engine.hh"

#include "BaseRenderer.hh"
#include "Engine/Graphics/D3D/D3DProfile.hh"
#include "Engine/Graphics/D3D11/D3D11Shader.hh"

#include <d3dcompiler.h>
#include <d3d11shader.h>

#define FIND_SHADER(type)                                                                                                                            \
    std::find_if(pData->Shaders.begin(), pData->Shaders.end(), [](const ShaderInfo &x) {                                                             \
        return x.Renderer == lr::IRenderer::CurrentAPI() && x.Type == ShaderType::type;                                                              \
    })

namespace lr
{
    void BaseShader::ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer)
    {
        ID3DBlob *pCode = 0;
        auto Compile = [&pCode, &pOutData](BufferStream &buf, LPCSTR entryPoint, ShaderType type) {
            constexpr u32 flags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;

            SAFE_RELEASE(pCode);
            ID3DBlob *pError = 0;

            D3DCompile(buf.GetData(), buf.GetSize(), entryPoint, nullptr, nullptr, entryPoint, D3D::GetLatestFeatureLevelString(type).data(), flags,
                       0, &pCode, &pError);

            if (pError) LOG_ERROR("Failed to compile shader!\n\n %s", (char *)pError->GetBufferPointer());

            SAFE_RELEASE(pError);

            pOutData->Type = type;
        };

        HRESULT hr;

        eastl::string shader((const char *)inBuffer.GetData(), inBuffer.GetSize());

        for (u8 i = 0; i < (u8)ShaderType::Count; i++)
        {
            switch ((ShaderType)i)
            {
                case ShaderType::Vertex:
                {
                    if (shader.find("VSMain") != -1) Compile(inBuffer, "VSMain", ShaderType::Vertex);
                    break;
                }

                case ShaderType::Pixel:
                {
                    if (shader.find("PSMain") != -1) Compile(inBuffer, "PSMain", ShaderType::Pixel);
                    break;
                }

                case ShaderType::Compute:
                {
                    if (shader.find("CSMain") != -1) Compile(inBuffer, "CSMain", ShaderType::Compute);
                    break;
                }

                default: break;
            }
        }

        pOutData->Renderer = RendererType::D3D11;
        pOutData->Buffer.Reset((u8 *)pCode->GetBufferPointer(), pCode->GetBufferSize());

        SAFE_RELEASE(pCode);
    }

    ShaderHandle Shader::Create(const Identifier &ident, const eastl::string &path, ShaderDesc *pDesc)
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

}  // namespace lr