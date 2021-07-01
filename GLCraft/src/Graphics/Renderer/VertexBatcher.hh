//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

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

#include <vector>

#include "Graphics/Shader.hh"

static const glm::mat4 g_m4DefPos = {
    1, 1, 1, 1,  // V1
    1, 0, 1, 1,  // V2
    0, 0, 1, 1,  // V3
    0, 1, 1, 1   // V4
};

static const glm::mat4x2 g_m4DefCoords = {
    1, 1,  // V1
    1, 0,  // V2
    0, 0,  // V3
    0, 1,  // V4
};

struct VertexInfo
{
    glm::vec3 Pos;
    glm::vec2 UV;
    glm::vec4 Color;
};

struct BatchEvent
{
    std::vector<VertexInfo> vertices{};
    uint32_t indexes = 0;
};

class VertexBatcher
{
public:
    VertexBatcher();
    ~VertexBatcher();

    void Init();
    void BeginFrame();
    void EndFrame();
    void Flush();
    void Reset();

private:
    std::vector<BatchEvent> m_vBatchEvents{};

private:
    BatchEvent &GetVertexData();

    uint32_t m_VBO;
    uint32_t m_IBO;

    uint32_t m_VAO;
};