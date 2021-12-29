//
// Created on Thursday 16th December 2021 by e-erdal
//

#pragma once

namespace Block_
{
    constexpr glm::mat4 kRectPosLUT[] = {
        { { 0, 1, 0, 1 }, { 1, 1, 0, 1 }, { 1, 0, 0, 1 }, { 0, 0, 0, 1 } },  // FRONT
        { { 0, 0, 1, 1 }, { 1, 0, 1, 1 }, { 1, 1, 1, 1 }, { 0, 1, 1, 1 } },  // BACK
        { { 0, 0, 1, 1 }, { 0, 1, 1, 1 }, { 0, 1, 0, 1 }, { 0, 0, 0, 1 } },  // RIGHT
        { { 1, 1, 1, 1 }, { 1, 0, 1, 1 }, { 1, 0, 0, 1 }, { 1, 1, 0, 1 } },  // LEFT
        { { 0, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 0, 1 }, { 0, 1, 0, 1 } },  // TOP
        { { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { 1, 0, 1, 1 }, { 0, 0, 1, 1 } },  // BOTTOM
    };

    constexpr glm::mat4x2 kQuadUV = {
        1, 0,  // V3
        0, 0,  // V4
        0, 1,  // V1
        1, 1,  // V2
    };

    constexpr glm::vec3 kNormalsLUT[] = { glm::vec3(1, 1, 1) };

}  // namespace Block_

struct BlockVertex
{
    glm::vec3 Position = {};
    glm::vec3 Normal = { 1, 1, 1 };
    glm::vec2 TexCoord = {};
    glm::vec4 Color = { 1, 1, 1, 1 };

    static void CalcFn(BlockVertex *pData, u32 i, const glm::mat4 &transform, const glm::mat4x2 &texCoord, const glm::ivec4 &color)
    {
    }
};

enum BlockFace : u8
{
    // OOOOOOXX
    BLOCK_FACE_NONE = 0,

    BLOCK_FACE_FRONT = 1 << 0,
    BLOCK_FACE_FRONT_I = 1,

    BLOCK_FACE_BACK = 1 << 1,
    BLOCK_FACE_BACK_I = 2,

    BLOCK_FACE_RIGHT = 1 << 2,
    BLOCK_FACE_RIGHT_I = 3,

    BLOCK_FACE_LEFT = 1 << 3,
    BLOCK_FACE_LEFT_I = 4,

    BLOCK_FACE_TOP = 1 << 4,
    BLOCK_FACE_TOP_I = 5,

    BLOCK_FACE_BOTTOM = 1 << 5,
    BLOCK_FACE_BOTTOM_I = 6,

    BLOCK_FACE_COUNT,
};
BitFlags(BlockFace);

constexpr inline BlockFace GetOppositeBlockFace(BlockFace face)
{
    switch (face)
    {
        case BLOCK_FACE_FRONT: return BLOCK_FACE_BACK;
        case BLOCK_FACE_RIGHT: return BLOCK_FACE_LEFT;
        case BLOCK_FACE_TOP: return BLOCK_FACE_BOTTOM;

        case BLOCK_FACE_BACK: return BLOCK_FACE_FRONT;
        case BLOCK_FACE_LEFT: return BLOCK_FACE_RIGHT;
        case BLOCK_FACE_BOTTOM: return BLOCK_FACE_TOP;
        default: break;
    }

    return BLOCK_FACE_NONE;
}

constexpr inline u32 GetBlockFaceCount(BlockFace faces)
{
    u32 v = (u32)faces;
    v = v - ((v >> 1) & 0x55555555);                         // reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);          // temp
    u32 c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;  // count
    return c;
}

struct Block
{
    u16 m_ID = 0;
    glm::ivec3 m_WorldPos = {};
    BlockFace m_CullingFace = BLOCK_FACE_NONE;
};
