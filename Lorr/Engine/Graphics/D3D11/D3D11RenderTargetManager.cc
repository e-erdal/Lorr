#if LR_BACKEND_D3D11

#include "D3D11RenderTargetManager.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"

namespace Lorr
{
    constexpr glm::vec4 color = glm::vec4(0, 0, 0, 0);

    void D3D11RenderTargetManager::Init(ID3D11Device *pDevice)
    {
        m_pDevice = pDevice;
    }

    void D3D11RenderTargetManager::ClearAll(ID3D11DeviceContext *pContext)
    {
        ZoneScoped;

        for (const auto &target : m_Targets) pContext->ClearRenderTargetView(target.second.pView, &color[0]);
    }

    ID3D11RenderTargetView *D3D11RenderTargetManager::Create(const Identifier &ident, u32 width, u32 height, TextureHandle pTargetTexture, u32 mipLevels)
    {
        ZoneScoped;

        HRESULT hr;
        auto it = m_Targets.find(ident);
        if (it == m_Targets.end())
        {
            TextureDesc desc;
            desc.Type = TEXTURE_TYPE_RENDER_TARGET;
            desc.MipMapLevels = mipLevels;

            TextureData data;
            data.Width = width;
            data.Height = height;
            data.Format = TextureFormat::TEXTURE_FORMAT_RGBAF32;

            if (!pTargetTexture) pTargetTexture = Texture::Create(ident, &desc, &data);
            D3D11Texture *d11Texture = (D3D11Texture *)pTargetTexture;

            RenderTarget target;
            target.pView = d11Texture->GetRenderTarget();
            target.Texture = pTargetTexture;
            target.textureData = data;

            m_Targets.emplace(ident, target);
            return target.pView;
        }

        return it->second.pView;
    }

    ID3D11RenderTargetView *D3D11RenderTargetManager::Create(const Identifier &ident, ID3D11Texture2D *pTexture)
    {
        ZoneScoped;

        HRESULT hr;
        auto it = m_Targets.find(ident);
        if (it == m_Targets.end())
        {
            ID3D11RenderTargetView *newTarget;
            if (FAILED(hr = m_pDevice->CreateRenderTargetView(pTexture, 0, &newTarget)))
            {
                LOG_ERROR("Failed to create new blend state into state manager stack!");
                return 0;
            }

            RenderTarget target;
            target.pView = newTarget;

            m_Targets.emplace(ident, target);
            return newTarget;
        }

        return it->second.pView;
    }

    ID3D11RenderTargetView *D3D11RenderTargetManager::GetView(const Identifier &ident)
    {
        ZoneScoped;

        auto it = m_Targets.find(ident);
        if (it == m_Targets.end()) return 0;

        return it->second.pView;
    }

    TextureHandle D3D11RenderTargetManager::GetTexture(const Identifier &ident)
    {
        ZoneScoped;

        auto it = m_Targets.find(ident);
        if (it == m_Targets.end()) return 0;

        return it->second.Texture;
    }

    void D3D11RenderTargetManager::Release(const Identifier &ident)
    {
        ZoneScoped;

        auto it = m_Targets.find(ident);
        if (it != m_Targets.end())
        {
            SAFE_RELEASE(it->second.pView);
            if (it->second.Texture) it->second.Texture->Delete();

            m_Targets.erase(it);
        }
    }
}  // namespace Lorr

#endif