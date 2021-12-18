//
// Created on Wednesday 22nd September 2021 by e-erdal
//

#pragma once

#include "Engine/Stream/BufferStream.hh"

namespace lr
{
    enum class ResourceType : u8
    {
        Invalid,
        Texture,
        Audio,
        Shader,
        Font,
        Model
    };

    struct IResourceDesc
    {
    };
    struct IResourceData
    {
    };

    template<typename TDesc = IResourceDesc, typename TData = IResourceData>
    class IResource
    {
    public:
        virtual void Init(const Identifier &ident, TDesc *pDesc, TData *pData) = 0;

        // Little hack to make our life easier
        static TDesc m_DescType;
        static TData m_DataType;

        static constexpr ResourceType m_ResType = ResourceType::Invalid;

    public:
        const Identifier &GetIdentifier() const
        {
            return m_Ident;
        }

    protected:
        Identifier m_Ident = kEmptyIdentifier;
    };

}  // namespace lr
