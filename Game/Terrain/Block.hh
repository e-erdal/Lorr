//
// Created on Thursday 16th December 2021 by e-erdal
//

#pragma once

//
//  BIT:
//      0 - FRONT
//      1 - BACK
//      2 - RIGHT
//      3 - LEFT
//      4 - TOP
//      5 - BOTTOM
//

constexpr u8 kBlockCullingLUT[] = {};

/*
      4
    #####
  3 # 0 # 2
    #####
      5   1 (Back)
*/

enum BlockFace : u8
{
    // OOOOOOXX
    BLOCK_FACE_NONE = 0,
    BLOCK_FACE_FRONT = 1 << 0,
    BLOCK_FACE_BACK = 1 << 1,
    BLOCK_FACE_RIGHT = 1 << 2,
    BLOCK_FACE_LEFT = 1 << 3,
    BLOCK_FACE_TOP = 1 << 4,
    BLOCK_FACE_BOTTOM = 1 << 5
};

struct Block
{
    u16 m_ID = 0;
    BlockFace m_CullingFace = BLOCK_FACE_NONE;
    
};
