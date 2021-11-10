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

        m_TotalMips = pDesc->MipMapLevels;
        m_Format = pData->Format;
        m_Width = pData->Width;
        m_Height = pData->Height;
        m_DataSize = pData->DataSize;

        HRESULT hr;

        LOG_INFO("Creating Texture2D <{}>({}, {})...", m_Ident, m_Width, m_Height);

        switch (pDesc->Type)
        {
            case TEXTURE_TYPE_REGULAR:
                CreateTexture2D(pData);
                CreateShaderResource();
                CreateSampler();
                break;
            case TEXTURE_TYPE_RENDER_TARGET:
                CreateRenderTarget();
                CreateShaderResource();
                CreateSampler();
                break;
            case TEXTURE_TYPE_DEPTH:
                CreateDepthTexture();
                if (m_Format == TEXTURE_FORMAT_R32_TYPELESS) m_Format = TEXTURE_FORMAT_R32_FLOAT;
                CreateShaderResource();
                CreateSampler();
                break;
            case TEXTURE_TYPE_RW:
                CreateRWTexture();
                CreateShaderResource();
                break;
        }

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

        SAFE_RELEASE(m_pUAV);
        SAFE_RELEASE(m_pRenderTarget);
    }

    void D3D11Texture::CreateTexture2D(TextureData *pData)
    {
        HRESULT hr;
        u32 miscFlags = 0;
        auto device = DX11Renderer->GetDevice();
        auto context = DX11Renderer->GetDeviceContext();

        if (m_TotalMips > 1) miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

        m_TextureDesc = {};
        m_TextureDesc.ArraySize = 1;
        m_TextureDesc.CPUAccessFlags = 0;
        m_TextureDesc.MipLevels = m_TotalMips;
        m_TextureDesc.MiscFlags = miscFlags;
        m_TextureDesc.SampleDesc.Quality = 0;
        m_TextureDesc.SampleDesc.Count = 1;
        m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        m_TextureDesc.Format = D3D::TextureFormatToDXFormat(m_Format);
        m_TextureDesc.Width = m_Width;
        m_TextureDesc.Height = m_Height;

        m_TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(hr = device->CreateTexture2D(&m_TextureDesc, 0, &m_pHandle)))
        {
            LOG_ERROR("Failed to create D3D11 texture!");
            return;
        }

        if (pData->Data) context->UpdateSubresource(m_pHandle, 0, 0, pData->Data, TextureFormatToSize(m_Format) * m_Width, 0);
    }

    void D3D11Texture::CreateDepthTexture()
    {
        HRESULT hr;
        auto device = DX11Renderer->GetDevice();
        auto context = DX11Renderer->GetDeviceContext();

        m_TextureDesc = {};
        m_TextureDesc.ArraySize = 1;
        m_TextureDesc.CPUAccessFlags = 0;
        m_TextureDesc.MipLevels = 1;
        m_TextureDesc.MiscFlags = 0;
        m_TextureDesc.SampleDesc.Quality = 0;
        m_TextureDesc.SampleDesc.Count = 1;
        m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        m_TextureDesc.Format = D3D::TextureFormatToDXFormat(m_Format);
        m_TextureDesc.Width = m_Width;
        m_TextureDesc.Height = m_Height;

        m_TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(hr = device->CreateTexture2D(&m_TextureDesc, 0, &m_pHandle)))
        {
            LOG_ERROR("Failed to create D3D11 texture!");
            return;
        }
    }

    void D3D11Texture::CreateRenderTarget()
    {
        HRESULT hr;
        u32 miscFlags = 0;
        auto device = DX11Renderer->GetDevice();
        auto context = DX11Renderer->GetDeviceContext();

        if (m_TotalMips > 1) miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

        m_TextureDesc = {};
        m_TextureDesc.ArraySize = 1;
        m_TextureDesc.CPUAccessFlags = 0;
        m_TextureDesc.MipLevels = m_TotalMips;
        m_TextureDesc.MiscFlags = miscFlags;
        m_TextureDesc.SampleDesc.Quality = 0;
        m_TextureDesc.SampleDesc.Count = 1;
        m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        m_TextureDesc.Format = D3D::TextureFormatToDXFormat(m_Format);
        m_TextureDesc.Width = m_Width;
        m_TextureDesc.Height = m_Height;

        m_TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(hr = device->CreateTexture2D(&m_TextureDesc, 0, &m_pHandle)))
        {
            LOG_ERROR("Failed to create D3D11 texture!");
            return;
        }

        D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
        viewDesc.Format = m_TextureDesc.Format;
        viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 1;
        viewDesc.Texture2D.MipSlice = 0;

        if (FAILED(hr = device->CreateRenderTargetView(m_pHandle, &viewDesc, &m_pRenderTarget)))
        {
            LOG_ERROR("Failed to create new blend state into state manager stack!");
            return;
        }
    }

    void D3D11Texture::CreateRWTexture()
    {
        HRESULT hr;
        auto device = DX11Renderer->GetDevice();
        auto context = DX11Renderer->GetDeviceContext();

        m_TextureDesc = {};
        m_TextureDesc.ArraySize = 1;
        m_TextureDesc.CPUAccessFlags = 0;
        m_TextureDesc.MipLevels = 1;
        m_TextureDesc.MiscFlags = 0;
        m_TextureDesc.SampleDesc.Quality = 0;
        m_TextureDesc.SampleDesc.Count = 1;
        m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        m_TextureDesc.Format = D3D::TextureFormatToDXFormat(m_Format);
        m_TextureDesc.Width = m_Width;
        m_TextureDesc.Height = m_Height;

        m_TextureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(hr = device->CreateTexture2D(&m_TextureDesc, 0, &m_pHandle)))
        {
            LOG_ERROR("Failed to create D3D11 texture!");
            return;
        }

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Texture2D.MipSlice = 0;

        if (FAILED(hr = device->CreateUnorderedAccessView(m_pHandle, &uavDesc, &m_pUAV)))
        {
            LOG_ERROR("Failed to create D3D11 UAV!");
            return;
        }
    }

    void D3D11Texture::CreateShaderResource()
    {
        ZoneScoped;

        HRESULT hr;
        ID3D11Device *pDevice = D3D11Renderer::Get()->GetDevice();
        ID3D11DeviceContext *pContext = D3D11Renderer::Get()->GetDeviceContext();

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = m_TotalMips;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Format = D3D::TextureFormatToDXFormat(m_Format);

        if (FAILED(hr = pDevice->CreateShaderResourceView(m_pHandle, &srvDesc, &m_pShaderResource)))
        {
            LOG_ERROR("Failed to create D3D11 texture shader resource view!");
            return;
        }

        if (m_TotalMips > 1) pContext->GenerateMips(m_pShaderResource);
    }

    void D3D11Texture::CreateSampler()
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