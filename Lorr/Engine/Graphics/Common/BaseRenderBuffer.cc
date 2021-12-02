#include "BaseRenderBuffer.hh"

#include "Engine/Graphics/D3D11/D3D11RenderBuffer.hh"

namespace Lorr
{
    RenderBufferHandle RenderBuffer::Create(const RenderBufferDesc &desc)
    {
        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11:
            {
                RenderBufferHandle buf = new D3D11RenderBuffer;
                buf->Init(desc);
                return buf;
            }

            default: break;
        }

        return 0;
    }

}  // namespace Lorr