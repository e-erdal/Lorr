#include "Renderer2D.hh"

namespace Lorr
{
    Renderer2D::~Renderer2D()
    {
        bgfx::shutdown();
    }

    void Renderer2D::Init( PlatformWindow *pWindow )
    {
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.platformData = pWindow->GetPlatformData();
        init.resolution.width = pWindow->GetWidth();
        init.resolution.height = pWindow->GetHeight();
        init.resolution.reset = m_ResetFlags;
        init.limits.transientVbSize = 134217728;

        if ( !bgfx::init( init ) )
        {
            Console::Fatal( "Failed to initialize BGFX." );
            return;
        }
    }

}  // namespace Lorr