#include "Texture2D.hh"

#include <bimg/decode.h>

#include "Engine/Stream/FileStream.hh"

namespace bgfx
{
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
}  // namespace bgfx

void Lorr::Texture2D::Init(const Identifier &ident, TEXTURE2D_DESC *pDesc, TEXTURE2D_DESC_SUBRESC *pSubRes)
{
    if (!pDesc) return;

    m_Ident = ident;

    // We have resource to process, no need to read file
    if (pSubRes)
    {
        m_Width = pSubRes->Width;
        m_Height = pSubRes->Height;
        m_DataSize = pSubRes->DataSize;

        LOG_TRACE("Loading a Texture2D from memory <{}>({}, {})", m_Ident, m_Width, m_Height);

        const auto *const pixelData = bgfx::copy(pSubRes->Data, pSubRes->DataSize);
        m_Handle = bgfx::createTexture2D(m_Width, m_Height, false, 1, pSubRes->Format, pDesc->Filters, pixelData);
    }
    else
    {
        Lorr::FileStream file(pDesc->Path, false);

        if (!file.IsOK())
        {
            LOG_ERROR("File not found! {}", pDesc->Path);
            return;
        }

        uint8_t *pData = 0;
        size_t dataSize = 0;

        pData = file.ReadAll<uint8_t>();
        dataSize = file.Size();

        auto *imageContainer = bimg::imageParse(bgfx::g_allocator, pData, dataSize);
        if (imageContainer == nullptr || !bgfx::isTextureValid(0, false, imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, 0))
        {
            LOG_ERROR("Cannot load image \"{}\"!", pDesc->Path);
            return;
        }

        LOG_TRACE("Loading a Texture2D from file <{}>({}, {})", m_Ident, imageContainer->m_width, imageContainer->m_height);

        m_Width = imageContainer->m_width;
        m_Height = imageContainer->m_height;
        m_Format = (bgfx::TextureFormat::Enum)imageContainer->m_format;
        m_DataSize = dataSize;

        const auto *const pixelData = bgfx::copy(imageContainer->m_data, imageContainer->m_size);
        m_Handle = bgfx::createTexture2D(m_Width, m_Height, 1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                                         (bgfx::TextureFormat::Enum)imageContainer->m_format, 0, pixelData);

        free(pData);
    }

    LOG_TRACE("Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height);

    std::string_view bgfxName = m_Ident.Raw();
    bgfx::setName(m_Handle, bgfxName.data(), bgfxName.length());
}

void Lorr::Texture2D::ParseMemory(TEXTURE2D_DESC_SUBRESC *pSubResc, BufferStream &buffer)
{
    auto *imageContainer = bimg::imageParse(bgfx::g_allocator, pSubResc->Data, pSubResc->DataSize);

    pSubResc->Width = imageContainer->m_width;
    pSubResc->Height = imageContainer->m_height;
    pSubResc->Format = (bgfx::TextureFormat::Enum)imageContainer->m_format;
    pSubResc->DataSize = imageContainer->m_size;

    buffer.InsertPtr((uint8_t *)imageContainer->m_data, imageContainer->m_size);
}