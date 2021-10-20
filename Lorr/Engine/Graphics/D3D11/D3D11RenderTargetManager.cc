#include "D3D11RenderTargetManager.hh"

namespace Lorr
{
    constexpr glm::vec4 color = glm::vec4(0, 0, 0, 0);

    void D3D11RenderTargetManager::Init(ID3D11Device *pDevice)
    {
        m_pDevice = pDevice;
    }

    void D3D11RenderTargetManager::ClearAll(ID3D11DeviceContext *pContext)
    {
        for (const auto &target : m_Targets) pContext->ClearRenderTargetView(target.second.pView, &color[0]);
    }

    void D3D11RenderTargetManager::ReInitAll(ID3D11DepthStencilView *pDepthStencil)
    {
        for (const auto &target : m_Targets)
        {
        }
    }

    ID3D11RenderTargetView *D3D11RenderTargetManager::Create(const Identifier &ident, uint32_t width, uint32_t height, TextureHandle pTargetTexture)
    {
        HRESULT hr;
        auto it = m_Targets.find(ident);
        if (it == m_Targets.end())
        {
            TextureDesc desc;
            TextureData data;
            desc.Type = TEXTURE_TYPE_RENDER_TARGET;
            data.Width = width;
            data.Height = height;

            if (!pTargetTexture) pTargetTexture = Texture::Create(ident, &desc, &data);

            D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
            viewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

            ID3D11RenderTargetView *newTarget;

            if (FAILED(hr = m_pDevice->CreateRenderTargetView((ID3D11Texture2D *)pTargetTexture->GetHandle(), &viewDesc, &newTarget)))
            {
                LOG_ERROR("Failed to create new blend state into state manager stack!");
                return 0;
            }

            RenderTarget target;
            target.pView = newTarget;
            target.Texture = pTargetTexture;
            target.textureData = data;

            m_Targets.emplace(ident, target);
            return newTarget;
        }

        return it->second.pView;
    }

    ID3D11RenderTargetView *D3D11RenderTargetManager::Create(const Identifier &ident, ID3D11Texture2D *pTexture)
    {
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
        auto it = m_Targets.find(ident);
        if (it == m_Targets.end()) return 0;

        return it->second.pView;
    }

    TextureHandle D3D11RenderTargetManager::GetTexture(const Identifier &ident)
    {
        auto it = m_Targets.find(ident);
        if (it == m_Targets.end()) return 0;

        return it->second.Texture;
    }

    void D3D11RenderTargetManager::Release(const Identifier &ident)
    {
        auto it = m_Targets.find(ident);
        if (it != m_Targets.end())
        {
            SAFE_RELEASE(it->second.pView);
            if (it->second.Texture) it->second.Texture->Delete();

            m_Targets.erase(it);
        }
    }
}  // namespace Lorr