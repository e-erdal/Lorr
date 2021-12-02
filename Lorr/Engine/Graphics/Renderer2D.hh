//
// Created on Sunday 14th November 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Managers/ShaderManager.hh"

namespace Lorr::Renderer2D
{
    void Init();
    void FullscreenQuad(TextureHandle texture, u32 textureSlot = 0);
    void FullscreenQuad(TextureHandle texture, ShaderProgram *pProgram, u32 textureSlot = 0);
    void FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot = 0);
    
}  // namespace Lorr::Renderer2D