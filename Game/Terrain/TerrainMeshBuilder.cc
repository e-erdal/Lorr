#include "TerrainMeshBuilder.hh"

#include "Engine/App/Engine.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Utils/Random.hh"

lr::InputLayout TerrainMeshBuilder::m_BlockLayout = {
    { lr::VertexAttribType::Vec3, "POSITION" },
    { lr::VertexAttribType::Vec3, "NORMAL" },
    { lr::VertexAttribType::Vec2, "TEXCOORD" },
    { lr::VertexAttribType::Vec4, "COLOR" },
};

struct TerrainCBufData
{
    glm::mat4 CameraMatrix;
};

void TerrainMeshBuilder::Init()
{
    ZoneScoped;

    auto pShaderMan = lr::GetEngine()->GetShaderMan();

    lr::RenderBufferDesc genericDynBufferDesc;
    genericDynBufferDesc.Type = lr::RenderBufferType::Constant;
    genericDynBufferDesc.Usage = lr::RenderBufferUsage::Dynamic;
    genericDynBufferDesc.MemFlags = lr::RenderBufferMemoryFlags::Access_CPUW;
    genericDynBufferDesc.DataLen = sizeof(TerrainCBufData);

    m_TerrainCBuffer = pShaderMan->CreateRenderBuffer("cbuffer://terrain", genericDynBufferDesc);
    m_pTerrainProgram = pShaderMan->CreateProgram("shader://terrain", m_BlockLayout, "shaders/terrainv.lr", "shaders/terrainp.lr");

    // m_TerrainTexture->GenerateMips();

    m_Batcher.Init(m_BlockLayout);
}

void TerrainMeshBuilder::Generate()
{
    ZoneScoped;

    for (u32 y = 0; y < kChunkHeight; y++)
    {
        for (u32 z = 0; z < kChunkWidth * kChunkCount; z++)
        {
            for (u32 x = 0; x < kChunkWidth * kChunkCount; x++)
            {
                Block *pBlock = PlaceBlock(glm::vec3(x, y, z));
                pBlock->m_ID = 1;
            }
        }
    }

    for (Chunk &chunk : m_Chunks) RefreshMesh(&chunk);
}

Block *TerrainMeshBuilder::PlaceBlock(const glm::ivec3 &pos)
{
    Chunk *pChunk = GetChunk(glm::ivec2(pos.x, pos.z));
    if (!pChunk) return nullptr;

    Block *pBlock = GetBlock(pChunk, glm::ivec3(pos.x, pos.y, pos.z));

    pBlock->m_WorldPos = pos;

    Chunk *pNextChunk = pChunk;
    Block *pSelectedBlock = nullptr;

    static auto Serialize = [&](const glm::ivec3 &targetPos, BlockFace faceToCull) {
        glm::ivec3 newPos = pos + targetPos;
        if (newPos.y < 0 || newPos.y >= kChunkHeight) return;

lambdaTop:
        if (pNextChunk && (pSelectedBlock = GetBlock(pNextChunk, newPos)))
        {
            pSelectedBlock->m_CullingFace |= faceToCull;
            pBlock->m_CullingFace |= GetOppositeBlockFace(faceToCull);
        }
        else
        {
            if ((pNextChunk = GetChunk(glm::ivec2(newPos.x, newPos.z), true))) goto lambdaTop;
        }
    };

    /// Handle culling
    Serialize(glm::ivec3(0, 0, 1), BLOCK_FACE_FRONT);
    Serialize(glm::ivec3(0, 0, -1), BLOCK_FACE_BACK);
    Serialize(glm::ivec3(1, 0, 0), BLOCK_FACE_RIGHT);
    Serialize(glm::ivec3(-1, 0, 0), BLOCK_FACE_LEFT);
    Serialize(glm::ivec3(0, -1, 0), BLOCK_FACE_TOP);
    Serialize(glm::ivec3(0, 1, 0), BLOCK_FACE_BOTTOM);

    return pBlock;
}

Block *TerrainMeshBuilder::GetBlock(Chunk *pChunk, const glm::ivec3 &pos)
{
    u32 chunkX = pChunk->Position.x * kChunkWidth;
    u32 chunkZ = pChunk->Position.y * kChunkWidth;

    u32 chunkSpaceX = pos.x - chunkX;  // pos in chunk space
    u32 chunkSpaceZ = pos.z - chunkZ;

    if (pos.x < chunkX || pos.z < chunkZ || chunkSpaceX >= kChunkWidth || chunkSpaceZ >= kChunkWidth) return nullptr;

    const u32 arrayPos = chunkSpaceX + (pos.y * kChunkWidth) + (chunkSpaceZ * kChunkHeight * kChunkWidth);
    return &pChunk->Blocks[arrayPos];
}

void TerrainMeshBuilder::Draw()
{
    ZoneScoped;

    lr::BaseRenderer *pRenderer = lr::GetEngine()->GetRenderer();

    TerrainCBufData terrainCBufData = { .CameraMatrix = lr::GetApp()->GetActiveScene()->GetEntity("entity://camera3d").GetCameraMatrix() };

    pRenderer->MapBuffer(m_TerrainCBuffer, &terrainCBufData, sizeof(TerrainCBufData));
    pRenderer->UnmapBuffer(m_TerrainCBuffer);

    pRenderer->UseConstantBuffer(m_TerrainCBuffer, lr::RenderBufferTarget::Vertex, 0);

    m_Batcher.SetCurrentProgram(m_pTerrainProgram);
    m_Batcher.SetCurrentTexture(pRenderer->GetPlaceholder());

    m_Batcher.Begin();
    for (Chunk &chunk : m_Chunks) m_Batcher.SetRects(chunk.pVertexData, chunk.RectCount);
    m_Batcher.End();
}

u32 TerrainMeshBuilder::GetVertexCount()
{
    ZoneScoped;

    u32 vertexCount = 0;
    for (Chunk &chunk : m_Chunks) vertexCount += chunk.RectCount * 4;

    return vertexCount;
}

Chunk *TerrainMeshBuilder::GetChunk(const glm::ivec2 &pos, bool noCreation)
{
    if (pos.x < 0 || pos.y < 0) return nullptr;

    const u32 posX = pos.x / kChunkWidth;
    const u32 posY = pos.y / kChunkWidth;

    for (Chunk &chunk : m_Chunks)
    {
        if (chunk.Position.x >= posX && chunk.Position.y >= posY && posX < chunk.Position.x + kChunkWidth && posY < chunk.Position.y + kChunkWidth)
            return &chunk;
    }

    if (noCreation) return nullptr;

    Chunk newChunk;
    newChunk.Position = glm::vec2(posX, posY);
    newChunk.Blocks.resize(kChunkWidth * kChunkWidth * kChunkHeight);
    m_Chunks.push_back(newChunk);

    LOG_TRACE("Created new chunk <{}, {}, {}>.", posX, posY, newChunk.Blocks.size());

    return &m_Chunks.back();
}

void TerrainMeshBuilder::RefreshMesh(Chunk *pChunk)
{
    static auto PushRect = [](BlockVertex *&pVertex, const glm::mat4 &transform, BlockFace face) {
        const auto &faceVert = Block_::kRectPosLUT[face - 1];
        for (u32 i = 0; i < 4; i++)
        {
            pVertex->Position = transform * faceVert[i];
            pVertex->Normal = glm::vec3(1, 1, 1);
            pVertex->TexCoord = Block_::kQuadUV[i];
            pVertex->Color = glm::vec4(1, 1, 1, 1);
            pVertex++;
        }
    };

    for (Block &block : pChunk->Blocks)
    {
        // TODO: hardcoded number, change it after implementing "block types"
        pChunk->RectCount += 6 - GetBlockFaceCount(block.m_CullingFace);
    }

    if (pChunk->pVertexData) free(pChunk->pVertexData);
    pChunk->pVertexData = (BlockVertex *)malloc(4 * pChunk->RectCount * sizeof(BlockVertex));
    BlockVertex *pVertex = pChunk->pVertexData;
    glm::mat4 posTransform = glm::mat4(1.0);

    for (Block &block : pChunk->Blocks)
    {
        posTransform = glm::translate(glm::mat4(1.0), glm::vec3(block.m_WorldPos));

        if (!(block.m_CullingFace & BLOCK_FACE_FRONT)) PushRect(pVertex, posTransform, BLOCK_FACE_FRONT_I);
        if (!(block.m_CullingFace & BLOCK_FACE_BACK)) PushRect(pVertex, posTransform, BLOCK_FACE_BACK_I);
        if (!(block.m_CullingFace & BLOCK_FACE_RIGHT)) PushRect(pVertex, posTransform, BLOCK_FACE_RIGHT_I);
        if (!(block.m_CullingFace & BLOCK_FACE_LEFT)) PushRect(pVertex, posTransform, BLOCK_FACE_LEFT_I);
        if (!(block.m_CullingFace & BLOCK_FACE_TOP)) PushRect(pVertex, posTransform, BLOCK_FACE_TOP_I);
        if (!(block.m_CullingFace & BLOCK_FACE_BOTTOM)) PushRect(pVertex, posTransform, BLOCK_FACE_BOTTOM_I);
    }
}