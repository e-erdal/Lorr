//
// Created on Sunday 2nd January 2022 by e-erdal
//

#pragma once

namespace lr
{

    constexpr u32 kCommandQueueCount = 3; // 1 - Direct, 2 - Copy, 3 - Compute
    class D3D12CommandQueueManager
    {
    public:
        D3D12CommandQueueManager() = default;

    private:
        eastl::array<ID3D12CommandQueue *, kCommandQueueCount> m_Queue;
    };

}  // namespace lr