//
// Created on Tuesday 16th November 2021 by e-erdal
//
//? GPU-based Silhouette + PCF shadow mapping
//? Ref:
//? - https://developer.amd.com/wordpress/media/2012/10/Isidoro-ShadowMapping.pdf - John Isidoro, AMD
//

#pragma once

#include "PostProcess/IPostProcess.hh"

#include "Common/IShader.hh"
#include "Common/IRenderBuffer.hh"
#include "Common/ITexture.hh"

namespace Lorr::PostProcess
{
    class ShadowMap : public IPostProcess
    {
    public:
        void Init() override;
        void Draw() override;

    private:
    };

}  // namespace Lorr::PostProcess