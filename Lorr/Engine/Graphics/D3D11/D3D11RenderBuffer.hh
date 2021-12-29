//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D11

#include "D3D11Renderer.hh"

#include "Engine/Graphics/Common/BaseRenderBuffer.hh"

namespace lr
{
    class D3D11RenderBuffer : public BaseRenderBuffer
    {
    public:
        void Init(const RenderBufferDesc &desc) override;

        void Delete() override;
        virtual ~D3D11RenderBuffer();

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

        D3D11_MAP GetMappingType()
        {
            return m_Mapping;
        }

    private:
        ID3D11Buffer *m_pHandle = 0;

        ID3D11ShaderResourceView *m_pSRV = 0;
        ID3D11UnorderedAccessView *m_pUAV = 0;

        D3D11_MAP m_Mapping;
    };
}  // namespace lr

#endif