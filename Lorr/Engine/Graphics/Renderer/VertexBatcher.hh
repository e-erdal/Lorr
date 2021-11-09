//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"
#include "Engine/Managers/ShaderManager.hh"

namespace Lorr
{
    struct BatcerCBuffer
    {
        glm::mat4 matrix;
    };

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
        glm::vec3 Position = {};
        glm::vec2 TexCoord = {};
        glm::vec4 Color = {};
    };

    class VertexBatcher
    {
    public:
        VertexBatcher() = default;

        void Init();

        void Begin();
        void End();

        void Flush();
        void Reset();

        void SetCurrentTexture(TextureHandle texture);
        void SetCurrentProgram(ShaderProgram *pProgram);

        void PushRect(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color = { 255, 255, 255, 255 });
        void PushRect(const glm::mat4 &transform, const glm::mat4x2 &uv, const glm::ivec4 &color = { 255, 255, 255, 255 });
        void PushRect(const glm::mat4 &transform, const glm::ivec4 &color = { 255, 255, 255, 255 });

    private:
        ShaderProgram *m_pShaderProgram = 0;
        
        RenderBufferHandle m_VertexBuffer = 0;
        RenderBufferHandle m_IndexBuffer = 0;

        TextureHandle m_CurrentTexture = 0;

        std::vector<BatcherVertex> m_Vertices;
        u32 m_Indexes = 0;

    public:
        static InputLayout m_Layout;
    };

}  // namespace Lorr