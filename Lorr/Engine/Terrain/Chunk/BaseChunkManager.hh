//
// Created on Thursday 20th January 2022 by e-erdal
//

#pragma once

#include "Engine/Managers/ShaderManager.hh"
#include "Engine/Graphics/Common/BaseRenderer.hh"

#include "Chunk.hh"

namespace lr
{
    /// Small trick to get over C++ templace mess
    template<typename ChunkDataT>
    struct BaseChunkRenderData
    {
        using ChunkRenderData = ChunkRenderData<ChunkDataT>;
        using ChunkData = ChunkDataT;
    };

    class BaseChunkManager
    {
    public:
        BaseChunkManager() = default;

        void Init(u32 width, u32 depth);

        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
        virtual void InitChunkRenderData(Chunk &chunk) = 0;

        Chunk *GetChunk(const glm::ivec2 &pos);
        void SubmitChunk(Chunk &chunk);

        struct ChunkRenderInfo
        {
            glm::vec3 m_LightDir = { 0.313, 0.969, 0 };
            float m_AmbientColor = 0.513;
        } m_RenderInfo;

    protected:
        virtual void InitRenderInfo() = 0;

    protected:
        glm::ivec2 m_TerrainSize = {};
        eastl::vector<Chunk> m_Chunks;
    };

}  // namespace lr
