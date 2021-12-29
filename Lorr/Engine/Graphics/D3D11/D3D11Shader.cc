#include "D3D11Shader.hh"

#include "Engine/Graphics/D3D/D3DType.hh"

namespace lr
{
    void D3D11Shader::Init(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData)
    {
        ZoneScoped;
        
        m_Type = pData->Type;

        HRESULT hr;
        auto *pDevice = D3D11Renderer::Get()->GetDevice();

        switch (m_Type)
        {
            case ShaderType::Vertex:
            {
                if (FAILED(hr = pDevice->CreateVertexShader(pData->Buffer.GetData(), pData->Buffer.GetSize(), 0, &m_Shader.m_pVertexShader)))
                {
                    LOG_ERROR("Failed to create D3D11 vertex shader!");
                    break;
                }

                eastl::vector<D3D11_INPUT_ELEMENT_DESC> elements;
                for (const auto &element : pDesc->Layout.GetElements())
                {
                    DXGI_FORMAT type = D3D::VertexTypeToDXFormat(element.m_Type);  // long ass d3d functions, had to satisfy clang format
                    elements.push_back({ element.m_Name.c_str(), 0, type, 0, element.m_Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
                }

                pDevice->CreateInputLayout(&elements[0], elements.size(), pData->Buffer.GetData(), pData->Buffer.GetSize(), &m_pLayout);
                break;
            }
            case ShaderType::Pixel:
            {
                if (FAILED(hr = pDevice->CreatePixelShader(pData->Buffer.GetData(), pData->Buffer.GetSize(), 0, &m_Shader.m_pPixelShader)))
                {
                    LOG_ERROR("Failed to create D3D11 pixel shader!");
                    break;
                }
                break;
            }
            case ShaderType::Compute:
            {
                if (FAILED(hr = pDevice->CreateComputeShader(pData->Buffer.GetData(), pData->Buffer.GetSize(), 0, &m_Shader.m_pComputeShader)))
                {
                    LOG_ERROR("Failed to create D3D11 compute shader!");
                    break;
                }
                break;
            }
        }
    }

}  // namespace lr