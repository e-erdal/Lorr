#include "ModelRendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Model/Model.hh"
#include "Engine/ECS/Components/TransformComponent.hh"

namespace lr::System
{
    void ModelRendererSystem::Init()
    {
        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

        //* Shaders *//
        RenderBufferDesc genericDynBufferDesc;
        genericDynBufferDesc.DataLen = sizeof(ModelRenderBuffer);
        genericDynBufferDesc.Type = RenderBufferType::Constant;
        genericDynBufferDesc.Usage = RenderBufferUsage::Dynamic;
        genericDynBufferDesc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

        pShaderMan->CreateRenderBuffer("cbuffer://model", genericDynBufferDesc);
        pShaderMan->CreateProgram("shader://model", Mesh::m_Layout, "shader:model.v", "shader:model.p");
    }

    void ModelRendererSystem::Tick(float deltaTime)
    {
    }

    void ModelRendererSystem::Draw()
    {
        ZoneScoped;

        // Engine components
        Engine *pEngine = GetEngine();
        BaseRenderer *pRenderer = pEngine->GetRenderer();
        TextureHandle postProcessTexture = pRenderer->GetRenderTargetTexture("renderer://postprocess");

        // Renderer resources
        ShaderProgram *modelProgram = pEngine->GetShaderMan()->GetProgram("shader://model");
        RenderBufferHandle modelCBuf = pEngine->GetShaderMan()->GetRenderBuffer("cbuffer://model");

        ModelRenderBuffer rbData;
        rbData.Matrix = GetApp()->GetActiveScene()->GetEntity("entity://camera3d").GetCameraMatrix();

        m_pRegistry->view<Model, Component::Transform>().each([&](auto entity, Model &model, Component::Transform &transform) {
            rbData.Position = transform.Position;
            pRenderer->MapBuffer(modelCBuf, &rbData, sizeof(ModelRenderBuffer));
            pRenderer->SetConstantBuffer(modelCBuf, RenderBufferTarget::Vertex, 0);

            pRenderer->SetShader(modelProgram->Vertex);
            pRenderer->SetShader(modelProgram->Pixel);

            model.Render();
        });
    }

}  // namespace lr::System