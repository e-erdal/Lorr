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
    }

}  // namespace Lorr