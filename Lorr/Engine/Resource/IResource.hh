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
        Texture
    };

    struct IRESOURCE_DESC
    {
    };

    struct IRESOURCE_DESC_SUBRESC
    {
    };

    template<typename TDesc = IRESOURCE_DESC, typename TSubResc = IRESOURCE_DESC_SUBRESC>
    class IResource
    {
    public:
        virtual void Init(const Identifier &ident, TDesc *pDesc, TSubResc *pSubResc = 0) = 0;
        virtual void ParseMemory(TSubResc *pSubResc, BufferStream &buffer) = 0;

        static constexpr ResourceType m_Type = ResourceType::Invalid;
    };

}  // namespace Lorr
