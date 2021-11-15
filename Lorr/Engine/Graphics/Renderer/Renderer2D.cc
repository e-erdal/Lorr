#include "Renderer2D.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    constexpr u16 kQuadIndices[] = { 0, 1, 2, 2, 3, 0 };

    struct QuadVertex
    {
        glm::vec3 Pos = {};
        glm::vec2 UV = {};
    };

    constexpr glm::mat4 kQuadVertices = {
        1,  -1, 0.5, 1,  // V0
        1,  1,  0.5, 1,  // V1
        -1, 1,  0.5, 1,  // V2
        -1, -1, 0.5, 1   // V3
    };

    constexpr glm::mat4x2 kQuadUV = {
        1, 1,  // V1
        1, 0,  // V2
        0, 0,  // V3
        0, 1,  // V4
    };

    struct Renderer2DData
    {
        InputLayout QuadLayout = {
            { VertexAttribType::Vec3, "POSITION" },
            { VertexAttribType::Vec2, "TEXCOORD" },
        };

        RenderBufferHandle QuadVertexBuffer = 0;
        RenderBufferHandle QuadIndexBuffer = 0;
        ShaderProgram *pQuadProgram = 0;
    };

    static Renderer2DData *pThis = 0;

    void Renderer2D::Init()
    {
        pThis = new Renderer2DData;

        QuadVertex pQuadVertex[4] = {
            { kQuadVertices[0], kQuadUV[0] },
            { kQuadVertices[1], kQuadUV[1] },
            { kQuadVertices[2], kQuadUV[2] },
            { kQuadVertices[3], kQuadUV[3] },
        };

        RenderBufferDesc quadVertexDesc;
        quadVertexDesc.pData = pQuadVertex;
        quadVertexDesc.DataLen = sizeof(pQuadVertex);
        quadVertexDesc.Type = RenderBufferType::Vertex;
        pThis->QuadVertexBuffer = RenderBuffer::Create(quadVertexDesc);

        RenderBufferDesc quadIndexDesc;
        quadIndexDesc.pData = (u16 *)kQuadIndices;
        quadIndexDesc.DataLen = sizeof(kQuadIndices);
        quadIndexDesc.Type = RenderBufferType::Index;
        pThis->QuadIndexBuffer = RenderBuffer::Create(quadIndexDesc);

        pThis->pQuadProgram =
            GetEngine()->GetShaderMan()->CreateProgram("renderer2d://fsq", pThis->QuadLayout, "shaders/fullscreenv.lr", "shaders/fullscreenp.lr");
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture)
    {
        IRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(pThis->pQuadProgram->Vertex);
        pRenderer->UseShader(pThis->pQuadProgram->Pixel);

        pRenderer->UseVertexBuffer(pThis->QuadVertexBuffer, &pThis->QuadLayout);
        pRenderer->UseIndexBuffer(pThis->QuadIndexBuffer, false);

        texture->Use();

        pRenderer->DrawIndexed(6);
    }

}  // namespace Lorr