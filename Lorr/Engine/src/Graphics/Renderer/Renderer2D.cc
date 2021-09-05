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
    }

    void Renderer2D::EndFrame()
    {
        bgfx::frame();
    }

}  // namespace Lorr