//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D11

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    class D3D11RenderBuffer : public IRenderBuffer
    {
    public:
        void Init(const RenderBufferDesc &desc) override;
        void SetData(void *pData, size_t dataLen) override;
        void *GetData() override;
        void UnmapData() override;
        void Delete() override;
        ~D3D11RenderBuffer();

    public:
        void *GetHandle() override
        {
            return m_pHandle;
        }

        ID3D11ShaderResourceView *GetShaderResource()
        {
            return m_pSRV;
        }

        ID3D11UnorderedAccessView *GetUAV()
        {
            return m_pUAV;
        }

    private:
        ID3D11Buffer *m_pHandle = 0;

        ID3D11ShaderResourceView *m_pSRV = 0;
        ID3D11UnorderedAccessView *m_pUAV = 0;

        D3D11_MAP m_Mapping;
        D3D11_MAPPED_SUBRESOURCE m_MappedResc;
    };
}  // namespace Lorr

#endif