//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    // The idea behind using glm functions:
    //
    // (pos)
    // vec4[4] =
    //     X X X X (X, Y, Z, W)
    //     X X X X
    //     X X X X
    //     X X X X = mat4x4
    //
    // (uv)
    // vec2[4] =
    //     X X . . (U, V)
    //     X X . .
    //     X X . .
    //     X X . . = mat4x2

    constexpr glm::mat4 kVertexPos = {
        1, 1, 1, 1,  // V1
        1, 0, 1, 1,  // V2
        0, 0, 1, 1,  // V3
        0, 1, 1, 1   // V4
    };

    constexpr glm::mat4x2 kVertexUV = {
        1, 1,  // V1
        1, 0,  // V2
        0, 0,  // V3
        0, 1,  // V4
    };

    struct BatcherVertex
    {
        glm::vec3 Pos;
        glm::vec2 UV;
        glm::vec4 Color;
    };
    typedef std::vector<BatcherVertex> BatcherVertices;

    struct BatcherEvent
    {
        BatcherVertices Vertices;
        uint32_t Indexes = 0;
    };
    typedef std::vector<std::pair<TextureHandle, BatcherEvent>> BatcherQueue;

    class VertexBatcher
    {
    public:
        VertexBatcher() = default;

        void Init();

        void Begin();
        void End();

        void Flush();
        void Reset();

        void PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::vec4 &uv,
                      const glm::ivec4 &color = { 255, 255, 255, 255 });
        void PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::mat4x2 &uv,
                      const glm::ivec4 &color = { 255, 255, 255, 255 });
        void PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::ivec4 &color = { 255, 255, 255, 255 });

    private:
        BatcherEvent &GetEvent(TextureHandle texture);

    private:
        ShaderHandle m_VertexShader;
        ShaderHandle m_PixelShader;
        RenderBufferHandle m_VertexBuffer;
        RenderBufferHandle m_IndexBuffer;
        RenderBufferHandle m_ConstantBuffer;
        InputLayout m_Layout;

        BatcherQueue m_Queue;
    };

}  // namespace Lorr