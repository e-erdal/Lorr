#include "ModelRendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Model/Model.hh"

namespace Lorr::System
{
    void ModelRendererSystem::Tick(float deltaTime)
    {
    }

    void ModelRendererSystem::Draw()
    {
        ZoneScoped;
        
        // Engine components
        Engine *pEngine = GetEngine();
        IRenderer *pRenderer = pEngine->GetRenderer();
        Camera3D *pCamera = pEngine->GetCamera3D();
        TextureHandle postProcessTexture = pRenderer->GetTargetTexture("renderer://postprocess");

        glm::mat4 cameraMatrix = glm::transpose(pCamera->GetProjection() * pCamera->GetView());

        // Renderer resources
        ShaderProgram *modelProgram = pEngine->GetShaderMan()->GetProgram("shader://model");
        RenderBufferHandle modelCBuf = pEngine->GetShaderMan()->GetRenderBuffer("cbuffer://model");

        pRenderer->SetCurrentTarget("renderer://postprocess");

        modelCBuf->SetData(&cameraMatrix[0][0], sizeof(glm::mat4));

        m_pRegistry->view<Model>().each([&](auto entity, Model &model) {
            pRenderer->UseShader(modelProgram->Vertex);
            pRenderer->UseShader(modelProgram->Pixel);
            pRenderer->UseConstantBuffer(modelCBuf, RenderBufferTarget::Vertex, 0);

            model.Render();
        });

        postProcessTexture->GenerateMips();
        pRenderer->SetCurrentTarget("renderer://backbuffer");
    }

}  // namespace Lorr::System