//
// Created on Thursday 16th December 2021 by e-erdal
//

#include "Block.hh"

#include "Engine/Graphics/VertexBatcher.hh"

class TerrainMeshBuilder
{
public:
    TerrainMeshBuilder() = default;

    void Init();

private:
    struct Chunk
    {
        std::array<Block, 16 * 256> Blocks;
    };

    std::vector<Chunk> m_Chunks;

    /// Renderer Components ///
};