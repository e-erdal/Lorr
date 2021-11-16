//
// Created on Monday 15th November 2021 by e-erdal
//
//? John Chapman style Pseudo lens flare technique, entirely taken from:
//? http://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
//

#pragma once

#include "PostProcess/IPostProcess.hh"

#include "Common/IShader.hh"
#include "Common/IRenderBuffer.hh"
#include "Common/ITexture.hh"

namespace Lorr::PostProcess
{
    class LensFlare : public IPostProcess
    {
    public:
        void Init() override;
        void Draw() override;

    private:
        //* Downsample Pass ----------- *
        struct DownsampleCB
        {
            float Scale;
            float Bias;
            float LOD;
            float ___padding;
        };
        DownsampleCB m_DownsampleCBData;
        RenderBufferHandle m_DownsampleCB;

        ShaderHandle m_DownsampleCS;
        TextureHandle m_UAVTexture;
        TextureHandle m_CSOutputTexture;
        //* --------------------------- *
        //* Lens Flare Pass ----------- *
        ShaderHandle m_LensFlarePS;
        //* --------------------------- *
    };

}  // namespace Lorr::PostProcess