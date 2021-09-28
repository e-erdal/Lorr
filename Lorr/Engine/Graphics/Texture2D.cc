#include "Texture2D.hh"

#include <bimg/decode.h>

#include "Engine/Stream/FileStream.hh"

namespace bgfx
{
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
}  // namespace bgfx

void Lorr::Texture2D::Init(const Identifier &ident, uint32_t Filters, Texture2DData *pData)
{
    m_Ident = ident;
    m_Filters = Filters;

    m_Width = pData->Width;
    m_Height = pData->Height;
    m_DataSize = pData->DataSize;

    LOG_TRACE("Loading a Texture2D from memory <{}>({}, {})", m_Ident, m_Width, m_Height);

    const auto *const pixelData = bgfx::copy(pData->Data, pData->DataSize);
    m_Handle = bgfx::createTexture2D(m_Width, m_Height, false, 1, pData->Format, m_Filters, pixelData);

    LOG_TRACE("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);

    std::string_view bgfxName = m_Ident.Raw();
    bgfx::setName(m_Handle, bgfxName.data(), bgfxName.length());
}

void Lorr::Texture2D::ParseToMemory(Texture2DData *pOutData, BufferStream &imageBuffer)
{
    auto *imageContainer = bimg::imageParse(bgfx::g_allocator, imageBuffer.GetData(), imageBuffer.GetSize());

    pOutData->Width = imageContainer->m_width;
    pOutData->Height = imageContainer->m_height;
    pOutData->Format = (bgfx::TextureFormat::Enum)imageContainer->m_format;
    pOutData->DataSize = imageContainer->m_size;
    pOutData->Data = (uint8_t *)imageContainer->m_data;
}