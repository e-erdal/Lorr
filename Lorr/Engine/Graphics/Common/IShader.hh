//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "InputLayout.hh"

#include "Engine/Graphics/Types.hh"
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
        RendererType Renderer;
        ShaderType Type;
        BufferStream Buffer;
    };

    class IShader : public IResource<ShaderDesc, ShaderData>
    {
    public:
        virtual void Init(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData) = 0;
        static void ParseToMemory(ShaderData *pOutData, BufferStream &inBuffer);

        static constexpr ResourceType m_ResType = ResourceType::Shader;

    public:
        const Identifier &GetIdentifier() const
        {
            return m_Ident;
        }

        ShaderType GetType()
        {
            return m_Type;
        }

    protected:
        Identifier m_Ident = kEmptyIdentifier;
        ShaderType m_Type = ShaderType::Vertex;
    };

    typedef IShader *ShaderHandle;

    namespace Shader
    {
        ShaderHandle Create(const Identifier &ident, const std::string &path, ShaderDesc *pDesc = 0);
        ShaderHandle Create(const Identifier &ident, ShaderDesc *pDesc, ShaderData *pData);
    }  // namespace Shader

}  // namespace Lorr
