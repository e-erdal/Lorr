//
// Created on Thursday 13th January 2022 by e-erdal
//

#include "BaseChunkManager.hh"

#include "PerlinNoise.hh"

namespace lr
{
    class DetailedChunkManager : public BaseChunkManager, public BaseChunkRenderData<ChunkDataDetailed>
    {
    public:
        DetailedChunkManager() = default;

        void Update(float deltaTime) override;
        void Render() override;
        void InitChunkRenderData(Chunk &chunk) override;

    protected:
        void InitRenderInfo() override;

    private:
        /// RENDER INFO ///
        ShaderProgram *m_pChunkProgram = nullptr;
        RenderBufferHandle m_ChunkVSBuffer = nullptr;
        RenderBufferHandle m_ChunkPSBuffer = nullptr;
        TextureHandle m_MainTexture = nullptr;

        siv::PerlinNoise m_Noise;
    };

}  // namespace lr