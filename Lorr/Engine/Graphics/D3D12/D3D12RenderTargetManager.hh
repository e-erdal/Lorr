//
// Created on Thursday 2nd December 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D12

#include "Engine/Graphics/Common/BaseRenderer.hh"

#include <d3d12.h>

namespace lr
{
    class D3D12RenderTargetManager
    {
    public:
        D3D12RenderTargetManager() = default;

        void Init();

    private:
        std::unordered_map<Identifier, ID3D12Resource *> m_RenderTargets;

    };

}  // namespace lr

#endif