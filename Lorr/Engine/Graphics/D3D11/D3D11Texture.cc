#if LR_BACKEND_D3D11

#include "D3D11Texture.hh"

#include "Engine/Graphics/D3D/D3DType.hh"

namespace Lorr
{
    void D3D11Texture::Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData)
    {
        ZoneScoped;

        m_Ident = ident;

        if (!pDesc && !pData) return;

        m_Width = pData->Width;
        m_Height = pData->Height;
        m_DataSize = pData->DataSize;

        HRESULT hr;

        LOG_INFO("Creating Texture2D <{}>({}, {})...", m_Ident, m_Width, m_Height);

        CreateHandle(pDesc, pData);
        CreateShaderResource(pDesc);
        CreateSampler(pDesc);

        LOG_INFO("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);
    }

    void D3D11Texture::Use()
    {
        ZoneScoped;

        ID3D11DeviceContext *pCtx = D3D11Renderer::Get()->GetDeviceContext();

        pCtx->PSSetShaderResources(0, 1, &m_pShaderResource);
        pCtx->PSSetSamplers(0, 1, &m_pSamplerState);
    }

    void *D3D11Texture::GetHandle()
    {
        return m_pHandle;
    }

    void D3D11Texture::Delete()
    {
        SAFE_RELEASE(m_pHandle);

        SAFE_RELEASE(m_pShaderResource);
        SAFE_RELEASE(m_pSamplerState);
        SAFE_RELEASE(m_pRenderTarget);
    }

    void D3D11Texture::CreateHandle(TextureDesc *pDesc, TextureData *pData)
    {
        ZoneScoped;

        m_Width = pData->Width;
        m_Height = pData->Height;
        m_DataSize = pData->DataSize;

        HRESULT hr;
        m_Format = D3D::TextureFormatToDXFormat(pData->Format);
        ID3D11Device *pDevice = D3D11Renderer::Get()->GetDevice();
        ID3D11DeviceContext *pContext = D3D11Renderer::Get()->GetDeviceContext();

        uint32_t miscFlags = 0;

        if (pDesc->MipMapLevels > 1)
        {
            miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
        }

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.ArraySize = 1;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MipLevels = pDesc->MipMapLevels;
        textureDesc.MiscFlags = miscFlags;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.Format = m_Format;
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;

        switch (pDesc->Type)
        {
            case TEXTURE_TYPE_DEPTH:
            {
                textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
                m_Format = DXGI_FORMAT_R32_FLOAT;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }
                break;
            }
            case TEXTURE_TYPE_RENDER_TARGET: m_Format = textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // didn't break for a reason
            case TEXTURE_TYPE_REGULAR:
            {
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }
                break;
            }
            case TEXTURE_TYPE_RW:
            {
                textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }

                D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
                uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
                uavDesc.Texture2D.MipSlice = 0;

                if (FAILED(hr = pDevice->CreateUnorderedAccessView(m_pHandle, &uavDesc, &m_pUAV)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }

                break;
            }

            default: break;
        }

        int pitch = 0;

        switch (pData->Format)
        {
            case TEXTURE_FORMAT_RGBA8: pitch = 4;
            default: break;
        }

        if (pData->Data) pContext->UpdateSubresource(m_pHandle, 0, 0, pData->Data, pitch * m_Width, 0);
    }

    void D3D11Texture::CreateShaderResource(TextureDesc *pDesc)
    {
        ZoneScoped;

        HRESULT hr;
        ID3D11Device *pDevice = D3D11Renderer::Get()->GetDevice();
        ID3D11DeviceContext *pContext = D3D11Renderer::Get()->GetDeviceContext();

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = pDesc->MipMapLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Format = m_Format;

        if (FAILED(hr = pDevice->CreateShaderResourceView(m_pHandle, &srvDesc, &m_pShaderResource)))
        {
            LOG_ERROR("Failed to create D3D11 texture shader resource view!");
            return;
        }

        if (pDesc->MipMapLevels > 1) pContext->GenerateMips(m_pShaderResource);
    }

    void D3D11Texture::CreateSampler(TextureDesc *pDesc)
    {
        ZoneScoped;

        HRESULT hr;
        ID3D11Device *pDevice = D3D11Renderer::Get()->GetDevice();

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        if (FAILED(hr = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState)))
        {
            LOG_ERROR("Failed to create D3D11 texture sampler!");
            return;
        }
    }

    void D3D11Texture::GenerateMips()
    {
        ZoneScoped;

        ID3D11DeviceContext *pContext = D3D11Renderer::Get()->GetDeviceContext();
        pContext->GenerateMips(m_pShaderResource);
    }

}  // namespace Lorr

#endif