//
// Created on Thursday 2nd December 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D12

#include "Engine/Graphics/Common/BaseRenderer.hh"

#include <d3d12.h>

namespace Lorr
{
    class D3D12RenderTargetManager
    {
    public:
        D3D12RenderTargetManager() = default;

        void Init();

    private:
        std::array<size_t, Renderer::Config::kMaxRenderTargets> m_Identifiers;
        std::array<ID3D12Resource *, Renderer::Config::kMaxRenderTargets> m_RenderTargets;

        ID3D12DescriptorHeap *m_pRTVHeap = nullptr;
    };

}  // namespace Lorr

#endif