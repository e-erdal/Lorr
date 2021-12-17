#include "D3D12RenderTargetManager.hh"

#include "D3D12Renderer.hh"

namespace Lorr
{
    void D3D12RenderTargetManager::Init()
    {
        auto pDevice = DX12Renderer->GetDevice();
        auto pSwapChain = DX12Renderer->GetSwapChain();
    }

}  // namespace Lorr