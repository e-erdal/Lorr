//
// Created on August 19th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/Shader/Shader.hh"

namespace Lorr
{
    class BGFXShader: public Shader
    {
    public:
        void Init(const Identifier &identifier, SHADER_DESC *pShaderD, SHADER_DESC_SUBRESC *pShaderSRES) override;

        void Use() override;
    };
}  // namespace Lorr