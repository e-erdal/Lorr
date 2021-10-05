//
// Created on Wednesday 22nd September 2021 by e-erdal
//

#pragma once

#include "Engine/Stream/BufferStream.hh"

namespace Lorr
{
    enum class ResourceType : uint8_t
    {
        Invalid,
        Texture,
        Audio,
        Shader,
        Font
    };

    class IResource
    {
    public:
        static constexpr ResourceType m_Type = ResourceType::Invalid;

    public:
        const Identifier &GetIdentifier() const
        {
            return m_Ident;
        }

    protected:
        Identifier m_Ident = kEmptyIdentifier;
    };

}  // namespace Lorr
