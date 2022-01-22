//
// Created on Friday 14th January 2022 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/BaseRenderBuffer.hh"
#include "Engine/Graphics/Common/InputLayout.hh"

namespace lr
{
    union ChunkDataPacked_P2x8_1x16
    {
        u32 PackedPos;
        struct
        {
            u16 PosY;
            u8 PosZ;
            u8 PosX;
        };

        static InputLayout m_Layout;
        static constexpr u32 m_kMaxVertex = 25;
    };

    struct ChunkDataDetailed
    {
        glm::vec3 Pos;
        glm::vec2 TexCoord;

        static InputLayout m_Layout;
        static constexpr i32 m_kMaxVertex = 25;
    };

    template<typename T>
    using ChunkRenderData = eastl::array<T, T::m_kMaxVertex * T::m_kMaxVertex * 4>;

    struct Chunk
    {
        glm::ivec2 m_Position = {};

        RenderBufferHandle m_VertexBuffer = nullptr;
        RenderBufferHandle m_IndexBuffer = nullptr;
        void *m_pRenderData = nullptr;
        u32 m_IndexCount = 0;
    };

}  // namespace lr