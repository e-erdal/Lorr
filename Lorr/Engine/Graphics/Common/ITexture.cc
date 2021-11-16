#include "ITexture.hh"

#include "Engine/App/Engine.hh"
#include "Engine/Graphics/Common/IRenderer.hh"
#include "Engine/Graphics/D3D11/D3D11Texture.hh"

#include <bx/allocator.h>
#include <bimg/decode.h>

BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459);  // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
static bx::DefaultAllocator s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();

namespace Lorr
{
    void ITexture::ParseToMemory(TextureData *pOutData, BufferStream &imageBuffer)
    {
        auto *imageContainer = bimg::imageParse(&s_allocator, imageBuffer.GetData(), imageBuffer.GetSize());

        pOutData->Width = imageContainer->m_width;
        pOutData->Height = imageContainer->m_height;
        // pOutData->Format = imageContainer->m_format;
        pOutData->DataSize = imageContainer->m_size;
        pOutData->Data = (u8 *)malloc(imageContainer->m_size);
        memcpy(pOutData->Data, imageContainer->m_data, imageContainer->m_size);

        bimg::imageFree(imageContainer);
    }

    TextureHandle Texture::Create(const Identifier &ident, const std::string &path, TextureDesc *pDesc)
    {
        switch (IRenderer::CurrentAPI())
        {
            case RendererType::D3D11: return GetEngine()->GetResourceMan()->LoadResource<D3D11Texture>(ident, path, pDesc);
            default: break;
        }

        return 0;
    }

    TextureHandle Texture::Create(const Identifier &ident, TextureDesc *pDesc, TextureData *pData)
    {
        switch (IRenderer::CurrentAPI())
        {
            case RendererType::D3D11:
            {
                D3D11Texture *texture = new D3D11Texture;

                texture->Init(ident, pDesc, pData);
                GetEngine()->GetResourceMan()->Append(texture);

                return (ITexture *)texture;
            }
            default: break;
        }

        return 0;
    }

}  // namespace Lorr
