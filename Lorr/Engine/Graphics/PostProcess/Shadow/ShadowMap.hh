//
// Created on Tuesday 16th November 2021 by e-erdal
//
//? GPU-based Silhouette + PCF shadow mapping
//? Ref:
//? - https://developer.amd.com/wordpress/media/2012/10/Isidoro-ShadowMapping.pdf - John Isidoro, AMD
//

#pragma once

#include "PostProcess/IPostProcess.hh"

#include "Common/BaseShader.hh"
#include "Common/BaseRenderBuffer.hh"
#include "Common/BaseTexture.hh"

namespace lr::PostProcess
{
    class ShadowMap : public IPostProcess
    {
    public:
        void Init() override;
        void Draw() override;

    private:
    };

}  // namespace lr::PostProcess