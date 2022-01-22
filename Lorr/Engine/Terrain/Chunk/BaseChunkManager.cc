#include "BaseChunkManager.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    void BaseChunkManager::Init(u32 width, u32 depth)
    {
        ZoneScoped;

        m_TerrainSize = { width, depth };

        InitRenderInfo();

        m_Chunks.resize(width * depth);
        for (u32 i = 0; i < width * depth; i++)
        {
            Chunk &chunk = m_Chunks[i];

            chunk.m_Position = { i % m_TerrainSize.x, i / m_TerrainSize.y };
            InitChunkRenderData(chunk);

            LOG_TRACE("Created new chunk<%d, %d>.", chunk.m_Position.x, chunk.m_Position.y);
        }
    }

    Chunk *BaseChunkManager::GetChunk(const glm::ivec2 &pos)
    {
        return nullptr;
    }

}  // namespace lr