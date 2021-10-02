#include "Renderer2D.hh"

namespace Lorr
{
    Renderer2D::~Renderer2D()
    {
    }

    void Renderer2D::Init(PlatformWindow *pWindow)
    {
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.platformData = pWindow->GetPlatformData();
        init.resolution.width = pWindow->GetWidth();
        init.resolution.height = pWindow->GetHeight();
        init.resolution.reset = m_ResetFlags;
        init.limits.transientVbSize = 134217728;

        if (!bgfx::init(init))
        {
            LOG_ERROR("Failed to initialize BGFX.");
            return;
        }

        m_ResWidth = pWindow->GetWidth();
        m_ResHeight = pWindow->GetHeight();

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f);
        bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

        constexpr uint32_t whiteColor = 0xffffffff;
        Texture2DData data;
        data.Width = 1;
        data.Height = 1;
        data.DataSize = sizeof(uint32_t);
        data.Data = (uint8_t *)&whiteColor;

        m_pPlaceholderTexture = new Texture2D;
        m_pPlaceholderTexture->Init("", &data, TEXTURE_MAG_NEAREST);
    }

    void Renderer2D::BeginFrame()
    {
        bgfx::touch(0);
    }

    void Renderer2D::EndFrame()
    {
        bgfx::frame();
    }

    void Renderer2D::SetViewTransform(uint32_t viewID, const glm::mat4 &proj, const glm::mat4 &view)
    {
        bgfx::setViewTransform(viewID, &proj[0][0], &view[0][0]);
    }

    void Renderer2D::SetTexture(Texture2D *pTexture, const bgfx::UniformHandle &uniform)
    {
        bgfx::setTexture(0, uniform, pTexture->GetHandle());
    }

    void Renderer2D::SetVSyncState(bool VSync)
    {
        m_ResetFlags |= BGFX_RESET_VSYNC;
        Reset();
    }

    void Renderer2D::Submit(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color)
    {
    }

    void Renderer2D::Reset()
    {
        bgfx::reset(m_ResWidth, m_ResHeight, m_ResetFlags);
    }

}  // namespace Lorr