//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Texture2D.hh"

struct VBInput
{
    glm::vec3 Pos;
    glm::vec2 UV;
    glm::vec3 Normal;
    glm::vec4 Color;
};

typedef std::vector<VBInput> BatcherVertices;
typedef std::vector<uint32_t> BatcherIndices;

class VertexBatcher
{
public:
    VertexBatcher() = default;

    void Init();

    void Begin(uint32_t preAllocRect = 0);
    void End();

    void PushRect();

public:
    BatcherVertices *GetVertices()
    {
        return &m_Vertices;
    }

    BatcherIndices *GetIndices()
    {
        return &m_Indices;
    }

private:
    void AllocRectangle(uint32_t count);

private:
    bgfx::VertexLayout m_Layout;

    BatcherVertices m_Vertices;
    BatcherIndices m_Indices;
    uint32_t m_Indexes = 0;

    bool m_Active = false;
};