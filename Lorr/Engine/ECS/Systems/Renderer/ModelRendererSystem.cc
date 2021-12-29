#include "ModelRendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Model/Model.hh"

namespace lr::System
{
    void ModelRendererSystem::Tick(float deltaTime)
    {
    }

    void ModelRendererSystem::Draw()
    {
        ZoneScoped;

        // Engine components
        Engine *pEngine = GetEngine();
        BaseRenderer *pRenderer = pEngine->GetRenderer();
        TextureHandle postProcessTexture = pRenderer->GetTargetTexture("renderer://postprocess");

        // Renderer resources
        ShaderProgram *modelProgram = pEngine->GetShaderMan()->GetProgram("shader://model");
        RenderBufferHandle modelCBuf = pEngine->GetShaderMan()->GetRenderBuffer("cbuffer://model");

        // pRenderer->SetCurrentTarget("renderer://postprocess");

        glm::mat4 cameraMatrix = GetApp()->GetActiveScene()->GetEntity("entity://camera3d").GetCameraMatrix();

        pRenderer->MapBuffer(modelCBuf, &cameraMatrix[0][0], sizeof(glm::mat4));
        pRenderer->UnmapBuffer(modelCBuf);

        m_pRegistry->view<Model>().each([&](auto entity, Model &model) {
            pRenderer->UseShader(modelProgram->Vertex);
            pRenderer->UseShader(modelProgram->Pixel);
            pRenderer->UseConstantBuffer(modelCBuf, RenderBufferTarget::Vertex, 0);

            model.Render();
        });

        // postProcessTexture->GenerateMips();
        // pRenderer->SetCurrentTarget("renderer://backbuffer");
    }

}  // namespace lr::System