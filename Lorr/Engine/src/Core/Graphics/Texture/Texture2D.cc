#include "Texture2D.hh"

#include "Core/Graphics/API/RendererAPI.hh"
#include "Core/Graphics/Texture/D3D11/D3D11Texture2D.hh"

namespace Lorr
{
    void Texture2D::Create(Texture2D **pOut, Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES)
    {
        switch (RendererAPI::CurrentAPI())
        {
        case APIType::D3D11:
        {
            D3D11Texture2D *texture = new D3D11Texture2D;

            texture->Init(Ident, pTextureD, pTextureSRES);

            pOut = (Texture2D **)&texture;
            break;
        }
        default: break;
        }
    }
}  // namespace Lorr
