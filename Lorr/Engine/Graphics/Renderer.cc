#include "Renderer.hh"

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace Lorr
{
    Renderer::~Renderer()
    {
    }

    void Renderer::Init(PlatformWindow *pWindow)
    {
        m_pHandle = new D3D11Renderer;
        if (!pWindow)  // Assuming it's console application
        {
            m_pHandle->Init(0, 0, 0);
            return;
        }

        m_ResWidth = pWindow->GetWidth();
        m_ResHeight = pWindow->GetHeight();

        m_pHandle->Init(pWindow, m_ResWidth, m_ResHeight);

        TextureDesc desc;
        desc.Filters = 0;

        constexpr uint32_t whiteColor = 0xffffffff;
        TextureData data;
        data.Width = 1;
        data.Height = 1;
        data.DataSize = sizeof(uint32_t);
        data.Data = (uint8_t *)&whiteColor;

        m_PlaceholderTexture = Texture::Create("batcher://placeholder", &desc, &data);
    }

    void Renderer::BeginFrame()
    {
        m_pHandle->SetClearColor({ 0, 0, 0, 1 });
    }

    void Renderer::EndFrame()
    {
        m_pHandle->Frame(0);
    }

    void Renderer::SetVSyncState(bool VSync)
    {
        Reset();
    }

    void Renderer::Submit(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color)
    {
    }

    void Renderer::Reset()
    {
    }

    void Renderer::SetView(uint32_t width, uint32_t height)
    {
        m_ResWidth = width;
        m_ResHeight = height;
        Reset();
    }

}  // namespace Lorr