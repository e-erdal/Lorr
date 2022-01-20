#include "BaseTexture.hh"

#include "Engine/App/Engine.hh"

#include "BaseRenderer.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"

#include <bx/allocator.h>
#include <bimg/decode.h>

BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459);  // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
static bx::DefaultAllocator s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();

namespace lr
{
    constexpr TextureFormat BGFXFormatToLRFormat(bimg::TextureFormat::Enum format)
    {
        switch (format)
        {
            case bimg::TextureFormat::RGBA8: return TextureFormat::RGBA8;
            case bimg::TextureFormat::RGBA16: return TextureFormat::RGBA16;
            case bimg::TextureFormat::RGBA32F: return TextureFormat::RGBA32F;
            case bimg::TextureFormat::R32F: return TextureFormat::R32F;
            case bimg::TextureFormat::D32F: return TextureFormat::DEPTH32F;
            case bimg::TextureFormat::D24S8: return TextureFormat::DEPTH24_STENCIL8;

            default: LOG_ERROR("Couldn't convert BGFX texture format to LR! Implement it! (Type: %u)", (u32)format); return TextureFormat::RGBA8;
        }
    }

    void BaseTexture::ParseToMemory(TextureData *pOutData, BufferStream &imageBuffer)
    {
        auto *imageContainer = bimg::imageParse(&s_allocator, imageBuffer.GetData(), imageBuffer.GetSize());

        pOutData->Width = imageContainer->m_width;
        pOutData->Height = imageContainer->m_height;
        pOutData->Format = BGFXFormatToLRFormat(imageContainer->m_format);
        pOutData->DataSize = imageContainer->m_size;
        pOutData->Data = (u8 *)malloc(imageContainer->m_size);
        memcpy(pOutData->Data, imageContainer->m_data, imageContainer->m_size);

        bimg::imageFree(imageContainer);
    }

    TextureHandle Texture::Create(const Identifier &ident, const eastl::string &path, TextureDesc *pDesc)
    {
        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11: return GetEngine()->GetResourceMan()->LoadResource<D3D11Texture>(ident, path, pDesc);
            default: break;
        }

        return 0;
    }

    TextureHandle Texture::Create(const Identifier &ident, TextureDesc *pDesc, TextureData *pData)
    {
        switch (BaseRenderer::CurrentAPI())
        {
            case RendererType::D3D11:
            {
                D3D11Texture *texture = new D3D11Texture;

                texture->Init(ident, pDesc, pData);
                GetEngine()->GetResourceMan()->Append(texture);

                return (BaseTexture *)texture;
            }
            default: break;
        }

        return 0;
    }

    TextureHandle Texture::Get(const Identifier &ident)
    {
        return GetEngine()->GetResourceMan()->GetResource<BaseTexture>(ident);
    }

}  // namespace lr
