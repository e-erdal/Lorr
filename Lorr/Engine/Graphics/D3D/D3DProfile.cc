#include "D3DProfile.hh"

namespace lr
{
    std::string_view D3D::VSFeatureLevelToString(D3D_FEATURE_LEVEL level)
    {
        switch (level)
        {
            case D3D_FEATURE_LEVEL_12_1:
            case D3D_FEATURE_LEVEL_12_0:
            {
                return "vs_6_0";
            }

            case D3D_FEATURE_LEVEL_11_1:
            case D3D_FEATURE_LEVEL_11_0:
            {
                return "vs_5_0";
            }

            case D3D_FEATURE_LEVEL_10_1:
            {
                return "vs_4_1";
            }

            case D3D_FEATURE_LEVEL_10_0:
            {
                return "vs_4_0";
            }

            case D3D_FEATURE_LEVEL_9_3:
            {
                return "vs_4_0_level_9_3";
            }

            case D3D_FEATURE_LEVEL_9_2:
            case D3D_FEATURE_LEVEL_9_1:
            {
                return "vs_4_0_level_9_1";
            }

            default: break;
        }

        return "";
    }

    std::string_view D3D::PSFeatureLevelToString(D3D_FEATURE_LEVEL level)
    {
        switch (level)
        {
            case D3D_FEATURE_LEVEL_12_1:
            case D3D_FEATURE_LEVEL_12_0:
            {
                return "ps_6_0";
            }

            case D3D_FEATURE_LEVEL_11_1:
            case D3D_FEATURE_LEVEL_11_0:
            {
                return "ps_5_0";
            }

            case D3D_FEATURE_LEVEL_10_1:
            {
                return "ps_4_1";
            }

            case D3D_FEATURE_LEVEL_10_0:
            {
                return "ps_4_0";
            }

            case D3D_FEATURE_LEVEL_9_3:
            {
                return "ps_4_0_level_9_3";
            }

            case D3D_FEATURE_LEVEL_9_2:
            case D3D_FEATURE_LEVEL_9_1:
            {
                return "ps_4_0_level_9_1";
            }

            default: break;
        }

        return "";
    }

    std::string_view D3D::CSFeatureLevelToString(D3D_FEATURE_LEVEL level)
    {
        switch (level)
        {
            case D3D_FEATURE_LEVEL_12_1:
            case D3D_FEATURE_LEVEL_12_0:
            {
                return "cs_6_0";
            }

            case D3D_FEATURE_LEVEL_11_1:
            case D3D_FEATURE_LEVEL_11_0:
            {
                return "cs_5_0";
            }

            case D3D_FEATURE_LEVEL_10_1:
            {
                return "cs_4_1";
            }

            case D3D_FEATURE_LEVEL_10_0:
            {
                return "cs_4_0";
            }

            case D3D_FEATURE_LEVEL_9_3:
            {
                return "cs_4_0_level_9_3";
            }

            case D3D_FEATURE_LEVEL_9_2:
            case D3D_FEATURE_LEVEL_9_1:
            {
                return "cs_4_0_level_9_1";
            }

            default: break;
        }

        return "";
    }

    std::string_view D3D::GetLatestFeatureLevelString(ShaderType type)
    {
        D3D_FEATURE_LEVEL level;

        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11: level = D3D11Renderer::Get()->GetDevice()->GetFeatureLevel(); break;
            case RendererType::D3D12: break;
            default: break;
        }

        switch (type)
        {
            case ShaderType::Vertex: return VSFeatureLevelToString(level);
            case ShaderType::Pixel: return PSFeatureLevelToString(level);
            case ShaderType::Compute: return CSFeatureLevelToString(level);
        }
        return "";
    }

}  // namespace lr