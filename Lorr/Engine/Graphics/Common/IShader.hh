//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "IRenderer.hh"
#include "InputLayout.hh"

#include "Engine/Resource/IResource.hh"

namespace Lorr
{
    enum class ShaderType : uint8_t
    {
        Vertex,
        Pixel,
        Compute
    };

    struct ShaderDesc
    {
        InputLayout Layout;
    };

    struct ShaderData
    {
        APIType Renderer;
        ShaderType Type;
        BufferStream Buffer;
    };

    class IShader : public IResource<ShaderDesc, ShaderData>
    {
    public:
        virtual void Init(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData) = 0;
        virtual void Use() = 0;
        static void ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer);

        static constexpr ResourceType m_ResType = ResourceType::Shader;

    public:
        const Identifier &GetIdentifier() const
        {
            return m_Ident;
        }

    protected:
        Identifier m_Ident = kEmptyIdentifier;
    };

    typedef IShader *ShaderHandle;

    namespace Shader
    {
        ShaderHandle Create(const Identifier &ident, const std::string &path, ShaderDesc *pDesc = 0);
        ShaderHandle Create(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData);
    }

}  // namespace Lorr
