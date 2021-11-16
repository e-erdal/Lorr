//
// Created on Sunday 14th November 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/IShader.hh"

namespace Lorr::Renderer2D
{
    void Init();
    void FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot = 0);
}