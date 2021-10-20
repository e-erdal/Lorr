#include "D3D11Texture.hh"

#include "Engine/Graphics/D3D/D3DType.hh"

namespace Lorr
{
    void D3D11Texture::Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData)
    {
        m_Ident = ident;
        m_Width = pData->Width;
        m_Height = pData->Height;
        m_DataSize = pData->DataSize;

        DXGI_FORMAT textureFormat = D3D::TextureFormatToDXFormat(pData->Format);
        ID3D11Device *pDevice = D3D11Renderer::Get()->GetDevice();
        HRESULT hr;

        LOG_INFO("Creating Texture2D <{}>({}, {})...", m_Ident, m_Width, m_Height);

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.ArraySize = 1;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MipLevels = 1;
        textureDesc.MiscFlags = 0;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.Format = textureFormat;
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;

        switch (pDesc->Type)
        {
            case TEXTURE_TYPE_DEPTH:
            {
                textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
                textureFormat = DXGI_FORMAT_R32_FLOAT;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }

                break;
            }
            case TEXTURE_TYPE_RENDER_TARGET:
            {
                textureFormat = textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }

                break;
            }
            case TEXTURE_TYPE_REGULAR:
            {
                textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

                // Initial definitons
                int pitch = 0;

                switch (pData->Format)
                {
                    case TEXTURE_FORMAT_RGBA8: pitch = 4;
                    default: break;
                }

                D3D11_SUBRESOURCE_DATA srd = {};
                srd.SysMemSlicePitch = 0;
                srd.pSysMem = pData->Data;
                srd.SysMemPitch = pitch * m_Width;

                if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, &srd, &m_pHandle)))
                {
                    LOG_ERROR("Failed to create D3D11 texture!");
                    return;
                }
                break;
            }
            default: break;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Format = textureFormat;

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;  // TODO: Add LOD levels
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        if (FAILED(hr = pDevice->CreateShaderResourceView(m_pHandle, &srvDesc, &m_pShaderResource)))
        {
            LOG_ERROR("Failed to create D3D11 texture shader resource view!");
            return;
        }

        if (FAILED(hr = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState)))
        {
            LOG_ERROR("Failed to create D3D11 texture sampler!");
            return;
        }

        LOG_INFO("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);
    }

    void D3D11Texture::Use()
    {
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

}  // namespace Lorr