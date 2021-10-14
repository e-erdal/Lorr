//
// Created on Tuesday 12th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace Lorr::D3D
{
    std::string_view GetLatestShaderFeature(ShaderType type);
    std::string_view GetLatestVSProfile(D3D_FEATURE_LEVEL level);
    std::string_view GetLatestPSProfile(D3D_FEATURE_LEVEL level);
}  // namespace Lorr::D3D