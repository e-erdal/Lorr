#ifdef LR_BACKEND_D3D12
#include "D3D12RenderCommand.hh"

namespace lr
{
    void D3D12RenderCommand::Init(ID3D12Device *pDevice, D3D12_COMMAND_LIST_TYPE type)
    {
        HRESULT hr;

        D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
        commandQueueDesc.Type = type;
        commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        commandQueueDesc.NodeMask = 1;  // Multiple GPU command submission.

        if (FAILED(hr = pDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_pQueue))))
        {
            LOG_ERROR("Failed to create D3D12 command queue!");
            return;
        }

        if (FAILED(hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence))))
        {
            LOG_ERROR("Failed to create D3D12 fence!");
            return;
        }

        for (u32 i = 0; i < Renderer::Config::kMaxFrameBuffers; i++)
        {
            CommandList &list = m_CommandLists[i];

            if (FAILED(hr = pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&list.pAllocator))))
            {
                LOG_ERROR("Failed to create D3D12 command allocator for frame %u!", i);
                return;
            }

            if (FAILED(hr = pDevice->CreateCommandList(0, type, list.pAllocator, nullptr /* PSO */, IID_PPV_ARGS(&list.pList))))
            {
                LOG_ERROR("Failed to create D3D12 command list!");
                return;
            }

            list.pList->Close();

            list.Event = CreateEventExA(nullptr, nullptr, false, EVENT_ALL_ACCESS);
        }
    }

    void D3D12RenderCommand::Record()
    {
        CommandList &list = m_CommandLists[m_WorkingFenceIndex];

        list.pAllocator->Reset();
        list.pList->Reset(list.pAllocator, nullptr);
    }

    void D3D12RenderCommand::Execute()
    {
        CommandList &list = m_CommandLists[m_WorkingFenceIndex];

        list.pList->Close();
        ID3D12CommandList *const commandLists[] = { list.pList };
        m_pQueue->ExecuteCommandLists(BX_COUNTOF(commandLists), commandLists);

        const u64 fenceValue = m_FenceValue;
        m_FenceValue++;
        m_pQueue->Signal(m_pFence, fenceValue);

        if (m_pFence->GetCompletedValue() < fenceValue)
        {
            m_pFence->SetEventOnCompletion(fenceValue, list.Event);
            WaitForSingleObject(list.Event, INFINITE);
            CloseHandle(list.Event);
        }

        m_WorkingFenceIndex = (m_WorkingFenceIndex + 1) % Renderer::Config::kMaxFrameBuffers;  // Ring buffer index implementation, use bx::RingBuffer instead
    }

}  // namespace lr

#endif