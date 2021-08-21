#include "Texture2D.hh"

#include "Core/Graphics/API/RendererAPI.hh"
#include "Core/Graphics/Texture/D3D11/D3D11Texture2D.hh"

namespace Lorr
{
    Texture2D *Texture2D::Create(Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES)
    {
        switch (RendererAPI::CurrentAPI())
        {
        case APIType::D3D11:  // bu da switch case, su an sadece bende directx 11 var
        {
            D3D11Texture2D *texture = new D3D11Texture2D;

            texture->Init(Ident, pTextureD, pTextureSRES);

            return (Texture2D *)texture;
            break;
        }
        default: break;
        }

        return 0;
    }
}  // namespace Lorr
