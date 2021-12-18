//
// Created on Sunday 14th November 2021 by e-erdal
//

#pragma once

#include "VertexBatcher.hh"

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Managers/ShaderManager.hh"

namespace lr
{
    struct Batcher2DBufferData
    {
        glm::mat4 Matrix;
    };

    class Renderer2D
    {
    public:
        Renderer2D() = default;

        void Init();
        void FullscreenQuad(TextureHandle texture, u32 textureSlot = 0);
        void FullscreenQuad(TextureHandle texture, ShaderProgram *pProgram, u32 textureSlot = 0);
        void FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot = 0);

    public:
        auto GetBatcher()
        {
            return &m_Batcher;
        }

    private:
        static constexpr u16 kQuadIndices[] = { 0, 1, 2, 2, 3, 0 };
        struct QuadVertex
        {
            glm::vec3 Pos = {};
            glm::vec2 UV = {};
        };

        static constexpr glm::mat4 kQuadVertices = {
            1,  -1, 0.5, 1,  // V0
            1,  1,  0.5, 1,  // V1
            -1, 1,  0.5, 1,  // V2
            -1, -1, 0.5, 1   // V3
        };

        static constexpr glm::mat4 kBatcher2DPos = {
            1, 1, 1, 1,  // V1
            1, 0, 1, 1,  // V2
            0, 0, 1, 1,  // V3
            0, 1, 1, 1   // V4
        };

        static constexpr glm::mat4x2 kQuadUV = {
            1, 1,  // V1
            1, 0,  // V2
            0, 0,  // V3
            0, 1,  // V4
        };

        struct Batcher2DVertex
        {
            glm::vec3 Position = {};
            glm::vec2 TexCoord = {};
            glm::vec4 Color = {};

            static void CalcFn(Batcher2DVertex *pData, u32 i, const glm::mat4 &transform, const glm::mat4x2 &texCoord, const glm::ivec4 &color)
            {
                pData->Position = transform * kBatcher2DPos[i];
                pData->TexCoord = texCoord[i];
                pData->Color = (glm::vec4)color / 255.f;
            }
        };

    private:
        InputLayout m_FullscreenQuadLayout = {
            { VertexAttribType::Vec3, "POSITION" },
            { VertexAttribType::Vec2, "TEXCOORD" },
        };

        RenderBufferHandle m_FullscreenQuadVB = nullptr;
        RenderBufferHandle m_FullscreenQuadIB = nullptr;
        ShaderHandle m_FullscreenVS = nullptr;
        ShaderHandle m_FullscreenPS = nullptr;

        static constexpr u32 kMaxVertex = 2000;
        VertexBatcher<Batcher2DVertex, kMaxVertex> m_Batcher;

    public:
        static InputLayout m_Batcher2DLayout;
    };

}  // namespace lr