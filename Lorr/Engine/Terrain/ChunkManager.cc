#include "ChunkManager.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    struct ChunkVSBufferInfo
    {
        glm::mat4 CameraMatrix;
        glm::vec2 ChunkPos;
        float padding[2];
    };

    struct ChunkPSBufferInfo
    {
        glm::vec3 LightDir;
        float AmbientColor;
    };

    void ChunkManager::Init(u32 width, u32 depth)
    {
        ZoneScoped;

        m_TerrainSize = { width, depth };

        const siv::PerlinNoise::seed_type seed = 3905806909;
        m_Noise.reseed(seed);

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

    void ChunkManager::InitRenderInfo()
    {
        ZoneScoped;

        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

        RenderBufferDesc bufferDesc;
        bufferDesc.Type = RenderBufferType::Constant;
        bufferDesc.Usage = RenderBufferUsage::Dynamic;
        bufferDesc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

        bufferDesc.DataLen = sizeof(ChunkVSBufferInfo);
        m_ChunkVSBuffer = pShaderMan->CreateRenderBuffer("cbuffer://terrain-vs", bufferDesc);

        bufferDesc.DataLen = sizeof(ChunkPSBufferInfo);
        m_ChunkPSBuffer = pShaderMan->CreateRenderBuffer("cbuffer://terrain-ps", bufferDesc);

        m_pChunkProgram = pShaderMan->CreateProgram("shader://terrain", ChunkData::m_Layout, "shader:terrain.v", "shader:terrain.p");

        TextureDesc tdesc;
        m_MainTexture = Texture::Create("terrain://grass", "texture:grass", &tdesc);
    }

    void ChunkManager::Update(float deltaTime)
    {
    }

    void ChunkManager::Render()
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        ChunkVSBufferInfo chunkVSData = { .CameraMatrix = GetApp()->GetActiveScene()->GetEntity("entity://camera3d").GetCameraMatrix() };
        ChunkPSBufferInfo chunkPSData = { .LightDir = m_RenderInfo.m_LightDir, .AmbientColor = m_RenderInfo.m_AmbientColor };

        pRenderer->SetShader(m_pChunkProgram->Vertex);
        pRenderer->SetShader(m_pChunkProgram->Pixel);

        m_MainTexture->Use();

        pRenderer->MapBuffer(m_ChunkPSBuffer, &chunkPSData, sizeof(ChunkPSBufferInfo));
        pRenderer->SetConstantBuffer(m_ChunkPSBuffer, RenderBufferTarget::Pixel, 0);

        for (Chunk &chunk : m_Chunks)
        {
            chunkVSData.ChunkPos = (glm::vec2)(chunk.m_Position * (ChunkData::m_kMaxVertex - 1));
            pRenderer->MapBuffer(m_ChunkVSBuffer, &chunkVSData, sizeof(ChunkVSBufferInfo));
            pRenderer->SetConstantBuffer(m_ChunkVSBuffer, RenderBufferTarget::Vertex, 0);

            pRenderer->SetVertexBuffer(chunk.m_VertexBuffer, &ChunkData::m_Layout);
            pRenderer->SetIndexBuffer(chunk.m_IndexBuffer, false);
            pRenderer->DrawIndexed(chunk.m_IndexCount);
        }
    }

    void ChunkManager::InitChunkRenderData(Chunk &chunk)
    {
        float chunkX = chunk.m_Position.x * (ChunkData::m_kMaxVertex - 1);
        float chunkZ = chunk.m_Position.y * (ChunkData::m_kMaxVertex - 1);

        chunk.m_pRenderData = new ChunkData[ChunkData::m_kMaxVertex * ChunkData::m_kMaxVertex];
        ChunkData *pRenderData = (ChunkData *)chunk.m_pRenderData;

        u16 *pIndices = new u16[ChunkData::m_kMaxVertex * ChunkData::m_kMaxVertex * 6];

        for (u32 z = 0; z < ChunkData::m_kMaxVertex; z++)
        {
            for (u32 x = 0; x < ChunkData::m_kMaxVertex; x++)
            {
                const float noise = m_Noise.octave2D_01((chunkX + x) * 0.008, (chunkZ + z) * 0.008, 5);
                pRenderData->Pos = { x, noise * 50, z };
                pRenderData->TexCoord = { x + 0.5, z + 0.5 };
                pRenderData->TexCoord /= (float)ChunkData::m_kMaxVertex;

                pRenderData++;
            }
        }

        for (u32 z = 0; z < ChunkData::m_kMaxVertex - 1; z++)
        {
            u32 depth = z * ChunkData::m_kMaxVertex;
            for (u32 x = 0; x < ChunkData::m_kMaxVertex - 1; x++)
            {
                pIndices[chunk.m_IndexCount + 0] = depth + x;
                pIndices[chunk.m_IndexCount + 1] = depth + x + 1;
                pIndices[chunk.m_IndexCount + 2] = depth + x + ChunkData::m_kMaxVertex + 1;

                pIndices[chunk.m_IndexCount + 3] = depth + x + ChunkData::m_kMaxVertex + 1;
                pIndices[chunk.m_IndexCount + 4] = depth + x + ChunkData::m_kMaxVertex;
                pIndices[chunk.m_IndexCount + 5] = depth + x;

                chunk.m_IndexCount += 6;
            }
        }

        RenderBufferDesc desc;
        desc.pData = chunk.m_pRenderData;
        desc.DataLen = ChunkData::m_kMaxVertex * ChunkData::m_kMaxVertex * sizeof(ChunkData);
        desc.Type = RenderBufferType::Vertex;
        chunk.m_VertexBuffer = RenderBuffer::Create(desc);

        desc.pData = pIndices;
        desc.DataLen = ChunkData::m_kMaxVertex * ChunkData::m_kMaxVertex * sizeof(u16) * 6;
        desc.Type = RenderBufferType::Index;

        chunk.m_IndexBuffer = RenderBuffer::Create(desc);

        delete[] pIndices;
    }

    Chunk *ChunkManager::GetChunk(const glm::ivec2 &pos)
    {
        return nullptr;
    }

}  // namespace lr