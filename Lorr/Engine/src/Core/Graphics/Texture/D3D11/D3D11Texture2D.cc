#include "D3D11Texture2D.hh"

#include "Core/Stream/FileStream.hh"

#include <stb_image.h>

namespace Lorr
{
    void D3D11Texture2D::Init(Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES)
    {
        if (!pTextureD) return;

        m_Ident = Ident;

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

        textureDesc.ArraySize = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MipLevels = 1;
        textureDesc.MiscFlags = 0;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
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
        uint8_t *pData = 0;

        // We have resource to process, no need to read file
        if (pTextureSRES)
        {
            m_Width = pTextureSRES->Width;
            m_Height = pTextureSRES->Height;
            m_DataSize = pTextureSRES->DataSize;

            srvDesc.Format = textureDesc.Format = ToDXFormat(pTextureSRES->Format);

            pData = pTextureSRES->Data;

            switch (pTextureSRES->Format)
            {
            case TEXTURE_FORMAT_RGBA8: pitch = 4;
            default: break;
            }
        }
        else
        {
            // TODO: This abstraction only reads png files, add support for DX texture files and others

            pData = stbi_load(pTextureD->Path.c_str(), (int *)&m_Width, (int *)&m_Height, &pitch, 0);
            m_DataSize = m_Width * pitch;

            srvDesc.Format = textureDesc.Format = ToDXFormat(TEXTURE_FORMAT_RGBA8);
        }

        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;

        D3D11_SUBRESOURCE_DATA srd;
        ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
        srd.SysMemSlicePitch = 0;
        srd.pSysMem = pData;
        srd.SysMemPitch = pitch * m_Width;

        Console::Info("Creating Texture2D <{}>({}, {})...", m_Ident, m_Width, m_Height);

        ID3D11Device *pDevice = D3D11API::Get()->GetDevice();

        HRESULT hr;
        hr = pDevice->CreateTexture2D(&textureDesc, &srd, &m_Handle);

        if (FAILED(hr))
        {
            PrintError("Failed to create D3D11 texture!");
            return;
        }

        hr = pDevice->CreateShaderResourceView(m_Handle, &srvDesc, &m_ShaderResource);

        if (FAILED(hr))
        {
            PrintError("Failed to create D3D11 texture shader resource view!");
            return;
        }

        hr = pDevice->CreateSamplerState(&samplerDesc, &m_SamplerState);

        if (FAILED(hr))
        {
            PrintError("Failed to create D3D11 texture sampler!");
            return;
        }

        // This is from file so we delete the memory
        if (!pTextureSRES) stbi_image_free(pData);

        Console::Info("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);
    }

}  // namespace Lorr