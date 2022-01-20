//
// Created on Wednesday 19th January 2022 by e-erdal
//

#pragma once

namespace lr
{
    enum class PrimitiveType
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip
    };

    enum class DepthFunc : u8
    {
        Never = 1,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    enum class Cull : u8
    {
        None = 1,
        Front,
        Back,
    };

    enum class SwapChainFlags : u8
    {
        None,
        vSync = 1 << 0,
        AllowTearing = 1 << 1,
        TripleBuffering = 1 << 2,
    };
    BitFlags(SwapChainFlags);
}  // namespace lr