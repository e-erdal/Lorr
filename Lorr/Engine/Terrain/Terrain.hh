//
// Created on Friday 14th January 2022 by e-erdal
//

#pragma once

#include "Chunk/BaseChunkManager.hh"

namespace lr
{
    enum class TerrainType
    {
        Detailed,
        Packed,
    };

    class Terrain
    {
    public:
        Terrain() = default;

        void Init(TerrainType type, u32 width, u32 depth);

        void Update(float deltaTime);
        void Render();

        void PickMouse(float x, float y);

    private:
        Identifier m_Ident = kEmptyIdentifier;

        BaseChunkManager *m_pChunkManager = nullptr;
    };

}  // namespace lr