//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/InputLayout.hh"

namespace Lorr
{
    enum class RenderBufferUsage
    {
        Default,    // GPU RW
        Dynamic,    // GPU R     CPU W
        Immutable,  // GPU R
        Staging     // GPU RW    CPU RW
    };

    enum RenderBufferAccess
    {
        RB_ACCESS_TYPE_NONE = 0,
        RB_ACCESS_TYPE_CPUW = 1 << 0,
        RB_ACCESS_TYPE_CPUR = 1 << 1
    };
    BitFlags(RenderBufferAccess);

    enum class RenderBufferType
    {
        Vertex,
        Index,
        Constant,
        ShaderResource
    };

    class IRenderBuffer
    {
    public:
        virtual void Init(void *pData, size_t dataLen, RenderBufferType type, RenderBufferUsage usage, RenderBufferAccess accessFlags) = 0;
        virtual void SetData(void *pData, size_t dataLen) = 0;
        virtual void Use(uint32_t offset = 0, InputLayout *pLayout = 0) = 0;
    };

    typedef IRenderBuffer *RenderBufferHandle;

    namespace RenderBuffer
    {
        RenderBufferHandle Create(void *pData, size_t dataLen, RenderBufferType type, RenderBufferUsage usage = RenderBufferUsage::Default,
                                  RenderBufferAccess accessFlags = RB_ACCESS_TYPE_NONE);
    }

}  // namespace Lorr