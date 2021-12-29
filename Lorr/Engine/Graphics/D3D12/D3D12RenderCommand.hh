//
// Created on Sunday 5th December 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D12

#include "Engine/Graphics/Common/BaseRenderer.hh"

namespace lr
{
    class D3D12RenderCommand
    {
    public:
        D3D12RenderCommand() = default;

        void Init(ID3D12Device *pDevice, D3D12_COMMAND_LIST_TYPE type);

        void Record();
        void Execute();

    private:
        struct CommandList
        {
            ID3D12CommandAllocator *pAllocator = nullptr;
            ID3D12GraphicsCommandList *pList = nullptr;
            HANDLE Event;
        };
        eastl::array<CommandList, Renderer::Config::kMaxFrameBuffers> m_CommandLists;

        u32 m_WorkingFenceIndex = 0;
        u64 m_FenceValue = 0;

        ID3D12Fence *m_pFence = nullptr;
        ID3D12CommandQueue *m_pQueue = nullptr;
    };

}  // namespace lr

#endif