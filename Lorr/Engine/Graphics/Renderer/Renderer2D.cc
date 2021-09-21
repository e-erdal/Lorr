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

        m_Batcher.Init();
    }

    void Renderer2D::BeginFrame()
    {
        bgfx::touch(0);
    }

    void Renderer2D::EndFrame()
    {
        bgfx::frame();
    }

    void Renderer2D::SetVSyncState(bool VSync)
    {
        m_ResetFlags |= BGFX_RESET_VSYNC;
    }

    void Renderer2D::Reset()
    {
        bgfx::reset(m_ResWidth, m_ResHeight, m_ResetFlags);
    }

}  // namespace Lorr