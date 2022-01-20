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

        m_FullscreenVS = pShaderMan->CreateShader("renderer2d://fscv", m_FullscreenQuadLayout, "shader:fullscreen.v");
        m_FullscreenPS = pShaderMan->CreateShader("renderer2d://fscp", "shader:fullscreen.p");

        m_Batcher.Init(m_Batcher2DLayout);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->SetShader(m_FullscreenVS);
        pRenderer->SetShader(m_FullscreenPS);

        pRenderer->SetVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->SetIndexBuffer(m_FullscreenQuadIB, false);

        pRenderer->SetShaderResource(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->SetSamplerState(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, ShaderHandle pixelShader, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->SetShader(m_FullscreenVS);
        pRenderer->SetShader(pixelShader);

        pRenderer->SetVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->SetIndexBuffer(m_FullscreenQuadIB, false);

        pRenderer->SetShaderResource(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->SetSamplerState(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

    void Renderer2D::FullscreenQuad(TextureHandle texture, ShaderProgram *pProgram, u32 textureSlot)
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        pRenderer->SetShader(pProgram->Vertex);
        pRenderer->SetShader(pProgram->Pixel);

        pRenderer->SetVertexBuffer(m_FullscreenQuadVB, &m_FullscreenQuadLayout);
        pRenderer->SetIndexBuffer(m_FullscreenQuadIB, false);

        pRenderer->SetShaderResource(texture, RenderBufferTarget::Pixel, textureSlot);
        pRenderer->SetSamplerState(texture, RenderBufferTarget::Pixel, textureSlot);

        pRenderer->DrawIndexed(6);
    }

}  // namespace lr