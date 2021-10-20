//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    class D3D11RenderBuffer : public IRenderBuffer
    {
    public:
        void Init(void *pData, size_t dataLen, RenderBufferType type, RenderBufferUsage usage, RenderBufferAccess accessFlags) override;
        void SetData(void *pData, size_t dataLen) override;
        void *GetNewData() override;
        void UnmapData() override;
        void Use(uint32_t offset, InputLayout *pLayout, bool index32) override;
        void Delete() override;

    private:
        ID3D11Buffer *m_pHandle = 0;

        RenderBufferType m_Type;

        D3D11_MAP m_Mapping;
        D3D11_MAPPED_SUBRESOURCE m_MappedResc;
    };
}  // namespace Lorr