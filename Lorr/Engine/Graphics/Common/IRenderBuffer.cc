#include "IRenderBuffer.hh"

#include "Engine/Graphics/D3D11/D3D11RenderBuffer.hh"

namespace Lorr
{
    RenderBufferHandle RenderBuffer::Create(void *pData, size_t dataLen, RenderBufferType type, RenderBufferUsage usage,
                                            RenderBufferAccess accessFlags)
    {
        switch (IRenderer::CurrentAPI())
        {
            case APIType::D3D11:
            {
                RenderBufferHandle buf = new D3D11RenderBuffer;
                buf->Init(pData, dataLen, type, usage, accessFlags);
                return buf;
            }

            default: break;
        }

        return 0;
    }

}  // namespace Lorr