//
// Created on Tuesday 12th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace lr::D3D
{
    std::string_view GetLatestFeatureLevelString(ShaderType type);

    std::string_view VSFeatureLevelToString(D3D_FEATURE_LEVEL level);
    std::string_view PSFeatureLevelToString(D3D_FEATURE_LEVEL level);
    std::string_view CSFeatureLevelToString(D3D_FEATURE_LEVEL level);
}  // namespace lr::D3D