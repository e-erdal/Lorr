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
        ShaderHandle VertexShader;
        ShaderHandle PixelShader;
    };

    static Renderer2DData *pThis = 0;

    void Renderer2D::Init()
    {
        ZoneScoped;

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

        pThis->VertexShader = GetEngine()->GetShaderMan()->CreateShader("renderer2d://fscv", pThis->QuadLayout, "shaders/fullscreenv.lr");
        pThis->PixelShader = GetEngine()->GetShaderMan()->CreateShader("renderer2d://fscp", "shaders/fullscreenp.lr");
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(pThis->VertexShader);
        pRenderer->UseShader(pThis->PixelShader);

        pRenderer->UseVertexBuffer(pThis->QuadVertexBuffer, &pThis->QuadLayout);
        pRenderer->UseIndexBuffer(pThis->QuadIndexBuffer, false);

        pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->UseSampler(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(pThis->VertexShader);
        pRenderer->UseShader(pixelShader);

        pRenderer->UseVertexBuffer(pThis->QuadVertexBuffer, &pThis->QuadLayout);
        pRenderer->UseIndexBuffer(pThis->QuadIndexBuffer, false);

        pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->UseSampler(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, ShaderProgram *pProgram, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(pProgram->Vertex);
        pRenderer->UseShader(pProgram->Pixel);

        pRenderer->UseVertexBuffer(pThis->QuadVertexBuffer, &pThis->QuadLayout);
        pRenderer->UseIndexBuffer(pThis->QuadIndexBuffer, false);

        pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->UseSampler(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

}  // namespace Lorr