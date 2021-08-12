#include "Texture2D.hh"

#include <bimg/bimg.h>

namespace Lorr
{
    namespace bgfx
    {
        // Hack for getting BGFX Allocator
        extern bx::AllocatorI *g_allocator;
    }  // namespace bgfx

    void Texture2D::Init( Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES )
    {
        m_Ident = Ident;

        // We have resource to process, no need to read file
        if ( pTextureSRES )
        {
        }
        else
        {
        }
    }

}  // namespace Lorr