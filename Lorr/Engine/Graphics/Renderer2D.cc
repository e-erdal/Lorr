#include "Renderer2D.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    InputLayout Renderer2D::m_Batcher2DLayout = {
        { VertexAttribType::Vec3, "POSITION" },
        { VertexAttribType::Vec2, "TEXCOORD" },
        { VertexAttribType::Vec4, "COLOR" },
    };

    void Renderer2D::Init()
    {
        ZoneScoped;

        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

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
        m_FullscreenQuadVB = RenderBuffer::Create(quadVertexDesc);

        RenderBufferDesc quadIndexDesc;
        quadIndexDesc.pData = (u16 *)kQuadIndices;
        quadIndexDesc.DataLen = sizeof(kQuadIndices);
        quadIndexDesc.Type = RenderBufferType::Index;
        m_FullscreenQuadIB = RenderBuffer::Create(quadIndexDesc);

        m_FullscreenVS = pShaderMan->CreateShader("renderer2d://fscv", m_FullscreenQuadLayout, "shaders/fullscreenv.lr");
        m_FullscreenPS = pShaderMan->CreateShader("renderer2d://fscp", "shaders/fullscreenp.lr");

        m_Batcher.Init(m_Batcher2DLayout);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(m_FullscreenVS);
        pRenderer->UseShader(m_FullscreenPS);

        pRenderer->UseVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->UseIndexBuffer(m_FullscreenQuadIB, false);

        pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->UseSampler(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->UseShader(m_FullscreenVS);
        pRenderer->UseShader(pixelShader);

        pRenderer->UseVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->UseIndexBuffer(m_FullscreenQuadIB, false);

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

        pRenderer->UseVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->UseIndexBuffer(m_FullscreenQuadIB, false);

        pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->UseSampler(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

}  // namespace lr