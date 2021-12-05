#include "D3D12RenderTargetManager.hh"

#include "D3D12Renderer.hh"

namespace Lorr
{
    void D3D12RenderTargetManager::Init()
    {
        auto pDevice = DX12Renderer->GetDevice();
        auto pSwapChain = DX12Renderer->GetSwapChain();

        /// Render target view heap
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = 2;  // Depends on D3D12->SwapChain->BufferCount
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pRTVHeap));

        u32 rtvHeapSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());

        for (u32 i = 0; i < heapDesc.NumDescriptors; i++)
        {
            pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
            pDevice->CreateRenderTargetView(m_RenderTargets[i], nullptr, rtvHandle);
            rtvHandle.Offset(1, rtvHeapSize);
        }
    }

}  // namespace Lorr