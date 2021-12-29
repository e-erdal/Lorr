//
// Created on Thursday 16th December 2021 by e-erdal
//

#include "Block.hh"

#include "Engine/Graphics/VertexBatcher.hh"

struct Chunk
{
    glm::ivec2 Position = {};

    std::vector<Block> Blocks;
    BlockVertex *pVertexData = nullptr;
    u32 RectCount = 0;
};

class TerrainMeshBuilder
{
public:
    TerrainMeshBuilder() = default;

    void Init();
    void Generate();

    /// Block handling
    Block *PlaceBlock(const glm::ivec3 &pos);
    Block *GetBlock(Chunk *pChunk, const glm::ivec3 &pos);

    /// Rendering
    void Draw();
    u32 GetVertexCount();

    /// Chunk handling
    Chunk *GetChunk(const glm::ivec2 &pos, bool noCreation = false);
    void RefreshMesh(Chunk *pChunk);

private:
    static constexpr u32 kChunkWidth = 16;
    static constexpr u32 kChunkHeight = 256;
    static constexpr u32 kChunkCount = 8;

    /// Renderer Components ///
    lr::VertexBatcher<BlockVertex, sizeof(BlockVertex) * 500> m_Batcher{};
    lr::RenderBufferHandle m_TerrainCBuffer = nullptr;
    lr::ShaderProgram *m_pTerrainProgram = nullptr;

    lr::TextureHandle m_TerrainTexture = nullptr;

    bool m_HighlightChunks = false;
    std::vector<Chunk> m_Chunks;

public:
    static lr::InputLayout m_BlockLayout;

    friend Block;
};