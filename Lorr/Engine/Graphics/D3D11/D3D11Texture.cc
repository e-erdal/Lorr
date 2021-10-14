#include "D3D11Texture.hh"

#include "Engine/Graphics/D3D/D3DType.hh"

namespace Lorr
{
    void D3D11Texture::Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData)
    {
        m_Ident = ident;
        m_Width = pData->Width;
        m_Height = pData->Height;

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

        //* Special texture creation
        if (textureFormat == DXGI_FORMAT_D32_FLOAT)  // If texture is a depth buffer
        {
            textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, 0, &m_Handle)))
            {
                LOG_ERROR("Failed to create D3D11 texture!");
                return;
            }

            LOG_INFO("Created a DepthTexture <{}>({}, {})!", m_Ident, m_Width, m_Height);
            return;
        }
        else  // If texture is regular shader texture
        {
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Format = textureFormat;

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.BorderColor[0] = 1.0f;
        samplerDesc.BorderColor[1] = 1.0f;
        samplerDesc.BorderColor[2] = 1.0f;
        samplerDesc.BorderColor[3] = 1.0f;
        samplerDesc.MinLOD = -FLT_MAX;  // TODO: Add LOD levels
        samplerDesc.MaxLOD = FLT_MAX;

        // Initial definitons
        int pitch = 0;

        switch (pData->Format)
        {
            case TEXTURE_FORMAT_RGBA8: pitch = 4;
            default: break;
        }

        D3D11_SUBRESOURCE_DATA srd;
        ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
        srd.SysMemSlicePitch = 0;
        srd.pSysMem = pData->Data;
        srd.SysMemPitch = pitch * m_Width;
        m_DataSize = pData->DataSize;

        if (FAILED(hr = pDevice->CreateTexture2D(&textureDesc, &srd, &m_Handle)))
        {
            LOG_ERROR("Failed to create D3D11 texture!");
            return;
        }

        if (FAILED(hr = pDevice->CreateShaderResourceView(m_Handle, &srvDesc, &m_ShaderResource)))
        {
            LOG_ERROR("Failed to create D3D11 texture shader resource view!");
            return;
        }

        if (FAILED(hr = pDevice->CreateSamplerState(&samplerDesc, &m_SamplerState)))
        {
            LOG_ERROR("Failed to create D3D11 texture sampler!");
            return;
        }

        LOG_INFO("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);
    }

    void *D3D11Texture::GetHandle()
    {
        return m_Handle;
    }

    void D3D11Texture::Delete()
    {
        SAFE_RELEASE(m_Handle);

        SAFE_RELEASE(m_ShaderResource);
        SAFE_RELEASE(m_SamplerState);
        SAFE_RELEASE(m_RenderTarget);
    }

}  // namespace Lorr