#if LR_BACKEND_D3D11

#include "D3D11RendererStateManager.hh"

namespace lr
{
    void D3D11RendererStateManager::SetDevice(ID3D11Device *pDevice)
    {
        m_pDevice = pDevice;
    }

    ID3D11RasterizerState *D3D11RendererStateManager::Get(D3D11_RASTERIZER_DESC &desc)
    {
        ZoneScoped;

        HRESULT hr;
        auto it = m_RasterizerStack.find(desc);
        if (it == m_RasterizerStack.end())
        {
            ID3D11RasterizerState *newRasterizer = 0;
            if (FAILED(hr = m_pDevice->CreateRasterizerState(&desc, &newRasterizer)))
            {
                LOG_ERROR("Failed to create new rasterizer state into state manager stack!");
                return 0;
            }

            m_RasterizerStack.emplace(desc, newRasterizer);
            return newRasterizer;
        }

        return it->second;
    }

    ID3D11DepthStencilState *D3D11RendererStateManager::Get(D3D11_DEPTH_STENCIL_DESC &desc)
    {
        ZoneScoped;

        HRESULT hr;
        auto it = m_DepthStack.find(desc);
        if (it == m_DepthStack.end())
        {
            ID3D11DepthStencilState *newDepthStencil = 0;
            if (FAILED(hr = m_pDevice->CreateDepthStencilState(&desc, &newDepthStencil)))
            {
                LOG_ERROR("Failed to create new depth stencil state into state manager stack!");
                return 0;
            }

            m_DepthStack.emplace(desc, newDepthStencil);
            return newDepthStencil;
        }

        return it->second;
    }

    ID3D11BlendState *D3D11RendererStateManager::Get(D3D11_BLEND_DESC &desc)
    {
        ZoneScoped;

        HRESULT hr;
        auto it = m_BlendStack.find(desc);
        if (it == m_BlendStack.end())
        {
            ID3D11BlendState *newBlend = 0;
            if (FAILED(hr = m_pDevice->CreateBlendState(&desc, &newBlend)))
            {
                LOG_ERROR("Failed to create new blend state into state manager stack!");
                return 0;
            }

            m_BlendStack.emplace(desc, newBlend);
            return newBlend;
        }

        return it->second;
    }

}  // namespace lr

#endif