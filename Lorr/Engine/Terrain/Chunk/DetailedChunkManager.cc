#include "DetailedChunkManager.hh"

#include "Engine/App/Engine.hh"
#include "Engine/Model/RT/EmbreeAPI.hh"

namespace lr
{
    struct ChunkVSBufferInfo
    {
        glm::mat4 CameraMatrix;
    };

    struct ChunkPSBufferInfo
    {
        glm::vec3 LightDir;
        float AmbientColor;
    };

    void DetailedChunkManager::InitRenderInfo()
    {
        ZoneScoped;

        m_Noise.reseed(2341241241);

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
        tdesc.MipMapLevels = 6;
        m_MainTexture = Texture::Create("terrain://grass", "texture:grass", &tdesc);
    }

    void DetailedChunkManager::Update(float deltaTime)
    {
    }

    void DetailedChunkManager::Render()
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

        pRenderer->MapBuffer(m_ChunkVSBuffer, &chunkVSData, sizeof(ChunkVSBufferInfo));
        pRenderer->SetConstantBuffer(m_ChunkVSBuffer, RenderBufferTarget::Vertex, 0);

        for (Chunk &chunk : m_Chunks)
        {
            pRenderer->SetVertexBuffer(chunk.m_VertexBuffer, &ChunkData::m_Layout);
            pRenderer->SetIndexBuffer(chunk.m_IndexBuffer);
            pRenderer->DrawIndexed(chunk.m_IndexCount);
        }
    }

    void DetailedChunkManager::InitChunkRenderData(Chunk &chunk)
    {
        float chunkX = chunk.m_Position.x * (ChunkData::m_kMaxVertex - 1);
        float chunkZ = chunk.m_Position.y * (ChunkData::m_kMaxVertex - 1);

        constexpr u32 kVertexSize = ChunkData::m_kMaxVertex * ChunkData::m_kMaxVertex;

        glm::vec3 *pRTPosAddr = nullptr;
        u32 *pRTIndicesAddr = nullptr;
        RT::NewGeometry(pRTPosAddr, kVertexSize, pRTIndicesAddr, kVertexSize * 6);
        glm::vec3 *pRTPos = pRTPosAddr;

        chunk.m_pRenderData = new ChunkData[kVertexSize];
        ChunkData *pRenderData = (ChunkData *)chunk.m_pRenderData;

        u32 *pIndices = new u32[kVertexSize * 6];

        for (u32 z = 0; z < ChunkData::m_kMaxVertex; z++)
        {
            for (u32 x = 0; x < ChunkData::m_kMaxVertex; x++)
            {
                float noise = m_Noise.octave2D_01((chunkX + x) * 0.01, (chunkZ + z) * 0.01, 5);
                *pRTPos = pRenderData->Pos = { chunkX + x, noise * 25, chunkZ + z };
                pRenderData->TexCoord = { x + 0.5, z + 0.5 };
                pRenderData->TexCoord /= (float)ChunkData::m_kMaxVertex;

                pRenderData++;
                pRTPos++;
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

        memcpy(pRTIndicesAddr, pIndices, kVertexSize * sizeof(u32) * 6);
        RT::CommitGeometry();

        RenderBufferDesc desc;

        desc.pData = pIndices;
        desc.DataLen = kVertexSize * sizeof(u32) * 6;
        desc.Type = RenderBufferType::Index;
        chunk.m_IndexBuffer = RenderBuffer::Create(desc);

        desc.pData = chunk.m_pRenderData;
        desc.DataLen = kVertexSize * sizeof(ChunkData);
        desc.Type = RenderBufferType::Vertex;
        desc.Usage = RenderBufferUsage::Dynamic;
        desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;
        chunk.m_VertexBuffer = RenderBuffer::Create(desc);

        delete[] pIndices;
    }

}  // namespace lr