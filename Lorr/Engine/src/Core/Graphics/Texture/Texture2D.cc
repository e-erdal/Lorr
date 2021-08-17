#include "Texture2D.hh"

#include "Core/Stream/FileStream.hh"

#include <bimg/decode.h>

namespace bgfx
{
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
}  // namespace bgfx

void Lorr::Texture2D::Init( Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES )
{
    if ( !pTextureD ) return;

    m_Ident = Ident;

    // We have resource to process, no need to read file
    if ( pTextureSRES )
    {
        m_Width = pTextureSRES->Width;
        m_Height = pTextureSRES->Height;
        m_DataSize = pTextureSRES->DataSize;

        Console::Info( "Loading a Texture2D from memory <{}>({}, {})", m_Ident, m_Width, m_Height );

        const auto *const pixelData = bgfx::copy( pTextureSRES->Data, pTextureSRES->DataSize );
        m_Handle = bgfx::createTexture2D( m_Width, m_Height, false, 1, pTextureSRES->Format, pTextureD->Filters, pixelData );
    }
    else
    {
        Lorr::FileStream file( pTextureD->Path, false );

        if ( !file.IsOK() )
        {
            Console::Error( "File not found! {}", pTextureD->Path );
            return;
        }

        uint8_t *pData = 0;
        size_t dataSize = 0;

        pData = file.ReadAll<uint8_t>();
        dataSize = file.Size();

        auto *imageContainer = bimg::imageParse( bgfx::g_allocator, pData, dataSize );
        if ( imageContainer == nullptr || !bgfx::isTextureValid( 0, false, imageContainer->m_numLayers, (bgfx::TextureFormat::Enum) imageContainer->m_format, 0 ) )
        {
            Console::Error( "Cannot load image \"{}\"!", pTextureD->Path );
            return;
        }

        Console::Info( "Loading a Texture2D from file <{}>({}, {})", m_Ident, imageContainer->m_width, imageContainer->m_height );

        m_Width = imageContainer->m_width;
        m_Height = imageContainer->m_height;
        m_DataSize = dataSize;

        const auto *const pixelData = bgfx::copy( imageContainer->m_data, imageContainer->m_size );
        m_Handle = bgfx::createTexture2D( m_Width, m_Height, 1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                                          (bgfx::TextureFormat::Enum) imageContainer->m_format, 0, pixelData );

        free( pData );
    }

    Console::Info( "Created a Texture2D <{}>({}, {})!", m_Ident, m_Width, m_Height );

    std::string_view bgfxName = m_Ident.Raw();
    bgfx::setName( m_Handle, bgfxName.data(), bgfxName.length() );
}
