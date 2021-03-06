//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/InputLayout.hh"

namespace lr
{
    enum class RenderBufferUsage : u8
    {
        Default,    // GPU RW
        Dynamic,    // GPU R     CPU W
        Immutable,  // GPU R
        Staging     // GPU RW    CPU RW
    };

    // MemoryFlags is the probably best naming for this, DataFlags looks weird
    enum class RenderBufferMemoryFlags : u16
    {
        None = 0,
        Structured = 1 << 0,
        AllowRawViews = 1 << 1,

        // Access
        Access_CPUW = 1 << 2,
        Access_CPUR = 1 << 3,

        // UAV
        UAV_Counter = 1 << 4,
        UAV_Append = 1 << 5
    };
    BitFlags(RenderBufferMemoryFlags);

    enum class RenderBufferType : u8
    {
        None,
        Vertex = 1 << 0,
        Index = 1 << 1,
        Constant = 1 << 2,
        ShaderResource = 1 << 3,
        UAV = 1 << 4
    };
    BitFlags(RenderBufferType);

    enum RenderBufferTarget : u8
    {
        Vertex = 1 << 0,
        Pixel = 1 << 1,
        Compute = 1 << 2,
    };
    BitFlags(RenderBufferTarget);

    struct RenderBufferDesc
    {
        RenderBufferType Type = RenderBufferType::None;
        RenderBufferUsage Usage = RenderBufferUsage::Default;
        RenderBufferMemoryFlags MemFlags = RenderBufferMemoryFlags::None;

        void *pData = nullptr;  // Data if available
        size_t DataLen = 0;

        u32 ByteStride = 0;
        u32 UAVElements = 0;  // The number of elements in the resource. For structured buffers, this is the number of structures in the buffer.
    };

    class BaseRenderBuffer
    {
    public:
        virtual void Init(const RenderBufferDesc &desc) = 0;

        virtual void *GetHandle() = 0;

        virtual void Delete() = 0;
        virtual ~BaseRenderBuffer(){};
    };

    typedef BaseRenderBuffer *RenderBufferHandle;

    namespace RenderBuffer
    {
        RenderBufferHandle Create(const RenderBufferDesc &desc);
    }

}  // namespace lr