#include "D3D12RenderTargetManager.hh"

#include "D3D12Renderer.hh"

namespace lr
{
    void D3D12RenderTargetManager::Init()
    {
        auto pDevice = DX12Renderer->GetDevice();
        auto pSwapChain = DX12Renderer->GetSwapChain();
    }

}  // namespace lr