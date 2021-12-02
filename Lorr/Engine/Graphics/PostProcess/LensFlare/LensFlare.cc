#include "LensFlare.hh"

#include "Engine/App/Engine.hh"

namespace Lorr::PostProcess
{
    void LensFlare::Init()
    {
        m_Name = "Lens Flare";

        ShaderManager *pShaderManager = GetEngine()->GetShaderMan();
        PlatformWindow *pWindow = GetEngine()->GetWindow();

        //* Downsample Pass ----------------------------------------------------------------------------------------------- *
        m_DownsampleCS = pShaderManager->CreateShader("postprocess://lensflare-downsample", "shaders/lensDownsample.lr");

        m_DownsampleCBData.Scale = 0.0;
        m_DownsampleCBData.Bias = 0.0;
        m_DownsampleCBData.LOD = 0.0;

        RenderBufferDesc downsampleData;
        downsampleData.pData = &m_DownsampleCBData;
        downsampleData.DataLen = sizeof(DownsampleCB);
        downsampleData.Type = RenderBufferType::Constant;
        m_DownsampleCB = pShaderManager->CreateRenderBuffer("postprocess://lensflare-downsample", downsampleData);

        TextureDesc textureDesc;
        textureDesc.Type = TEXTURE_TYPE_RW;

        TextureData textureData;
        textureData.Format = TEXTURE_FORMAT_RGBA32F;
        textureData.Width = pWindow->GetWidth();
        textureData.Height = pWindow->GetHeight();
        m_UAVTexture = Texture::Create("postprocess://lensflare-downsample-rw", &textureDesc, &textureData);

        textureDesc.Type = TEXTURE_TYPE_REGULAR;
        m_CSOutputTexture = Texture::Create("postprocess://lensflare-downsample-output", &textureDesc, &textureData);
        //* --------------------------------------------------------------------------------------------------------------- *
    }

    void LensFlare::Draw()
    {
        BaseRenderer *pRenderer = GetEngine()->GetRenderer();
        TextureHandle postProcView = pRenderer->GetTargetTexture("renderer://postprocess");

        //* Downsample Pass ----------------------------------------------------------------------------------------------- *
        pRenderer->UseShader(m_DownsampleCS);
        
        pRenderer->UseUAV(m_UAVTexture, RenderBufferTarget::Compute, 0);
        pRenderer->UseShaderBuffer(postProcView, RenderBufferTarget::Compute, 0);
        pRenderer->UseSampler(postProcView, RenderBufferTarget::Compute, 0);

        pRenderer->Dispatch(ceil(m_UAVTexture->GetWidth() / 8), ceil(m_UAVTexture->GetHeight() / 8), 1);
        pRenderer->TransferResourceData(m_UAVTexture, m_CSOutputTexture);

        pRenderer->UseShaderBuffer((TextureHandle)0, RenderBufferTarget::Compute, 0);
        pRenderer->UseUAV((TextureHandle)0, RenderBufferTarget::Compute, 0);
        //* --------------------------------------------------------------------------------------------------------------- *
    }

}  // namespace Lorr::PostProcess
