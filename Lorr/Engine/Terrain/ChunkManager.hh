//
// Created on Thursday 13th January 2022 by e-erdal
//

#include "Chunk.hh"

#include "Engine/Graphics/VertexBatcher.hh"

#include "PerlinNoise.hh"

namespace lr
{
    /// Small trick to get over C++ templace mess
    template<typename ChunkDataT>
    struct BaseChunkRenderData
    {
        using ChunkRenderData = ChunkRenderData<ChunkDataT>;
        using ChunkData = ChunkDataT;
    };
    
    ///                  we apply template trick here
    class ChunkManager : public BaseChunkRenderData<ChunkDataDetailed>
    {
    public:
        ChunkManager() = default;

        void Init(u32 width, u32 depth);

        void Update(float deltaTime);
        void Render();

        void InitChunkRenderData(Chunk &chunk);
        Chunk *GetChunk(const glm::ivec2 &pos);
        void SubmitChunk(Chunk &chunk);

        struct ChunkRenderInfo
        {
            glm::vec3 m_LightDir = { 0.313, 0.969, 0 };
            float m_AmbientColor = 0.513;
        } m_RenderInfo;

    private:
        void InitRenderInfo();

    private:
        siv::PerlinNoise m_Noise;
        glm::ivec2 m_TerrainSize = {};
        eastl::vector<Chunk> m_Chunks;

        /// RENDER INFO ///
        ShaderProgram *m_pChunkProgram = nullptr;
        RenderBufferHandle m_ChunkVSBuffer = nullptr;
        RenderBufferHandle m_ChunkPSBuffer = nullptr;
        TextureHandle m_MainTexture = nullptr;
    };

}  // namespace lr