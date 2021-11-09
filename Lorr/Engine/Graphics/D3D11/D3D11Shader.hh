//
// Created on Wednesday 13th October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D11

#include "Engine/Graphics/Common/IShader.hh"

#include "D3D11Renderer.hh"

namespace Lorr
{
    class D3D11Shader : public IShader
    {
    public:
        void Init(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData) override;

    private:
        union
        {
            ID3D11VertexShader *m_pVertexShader = 0;
            ID3D11PixelShader *m_pPixelShader;
            ID3D11ComputeShader *m_pComputeShader;
        } m_Shader;

        ID3D11InputLayout *m_pLayout;  // Input layout if shader type is Vertex

    public:
        const auto &GetShader() const
        {
            return m_Shader;
        }

        ID3D11InputLayout *GetLayout()
        {
            return m_pLayout;
        }
    };

}  // namespace Lorr

#endif