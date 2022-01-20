#include "GameApp.hh"
#include <bitset>

#include "Engine/Terrain/ChunkManager.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/CameraComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Graphics/Font.hh"
#include "Engine/Graphics/Renderer2D.hh"

using namespace lr;

TextureHandle texture;
Font *pFont;
ShaderProgram *fontShader;

lr::ChunkManager chunkMan;

void GameApp::Init()
{
    ZoneScoped;

    LoadResources();

    //* Required vars
    BaseRenderer *pRenderer = GetEngine()->GetRenderer();
    BaseWindow *pWindow = GetEngine()->GetWindow();

    pWindow->OnResolutionChanged.connect<&GameApp::OnResolutionChanged>(this);

    u32 width = pWindow->GetWidth();
    u32 height = pWindow->GetHeight();

    //* Scene initialization
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("scene://default-scene");

    // Post-process target
    pRenderer->CreateRenderTarget("renderer://postprocess", width, height, 0);
    pRenderer->CreateRenderTarget("renderer://shadowmap", 512, 512, 0);

    //** Init entities **//
    Entity camera3D = m_pCurrentScene->CreateEntity("entity://camera3d");
    camera3D.AttachCamera3D(glm::vec3(0, 0, -5), glm::vec2(width, height), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 60.f, 0.1f, 10000.f);

    Entity camera2D = m_pCurrentScene->CreateEntity("entity://camera2d");
    camera2D.AttachCamera2D(glm::vec2(0, 0), glm::vec2(width, height));

    chunkMan.Init(1, 1);
}

void GameApp::Tick(float deltaTime)
{
    ZoneScoped;

    m_pCurrentScene->Tick(deltaTime);

    chunkMan.Update(deltaTime);
}

bool wireframe = false;

void GameApp::Draw()
{
    ZoneScoped;

    m_pCurrentScene->Draw();
    BaseRenderer *pRenderer = GetEngine()->GetRenderer();
    ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

    ImGui::Begin("GameApp", nullptr);
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);

    ImGui::SliderFloat3("Light Dir", &chunkMan.m_RenderInfo.m_LightDir[0], -1, 1);
    ImGui::SliderFloat("Ambient Color", &chunkMan.m_RenderInfo.m_AmbientColor, 0, 1);

    if (ImGui::Checkbox("Wireframe", &wireframe))
    {
        pRenderer->SetWireframeState(wireframe);
    }

    ImGui::End();

    chunkMan.Render();
}

void GameApp::LoadResources()
{
    ZoneScoped;

    auto shaderMan = GetEngine()->GetShaderMan();
    auto resourceMan = GetEngine()->GetResourceMan();

    //* Shaders *//
    RenderBufferDesc genericDynBufferDesc;
    genericDynBufferDesc.Type = RenderBufferType::Constant;
    genericDynBufferDesc.Usage = RenderBufferUsage::Dynamic;
    genericDynBufferDesc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

    // Model constant buffer
    genericDynBufferDesc.DataLen = sizeof(ModelRenderBuffer);
    shaderMan->CreateRenderBuffer("cbuffer://model", genericDynBufferDesc);
    shaderMan->CreateProgram("shader://model", Mesh::m_Layout, "shader:model.v", "shader:model.p");

    // VertexBatcher constant buffer
    genericDynBufferDesc.DataLen = sizeof(Batcher2DBufferData);
    shaderMan->CreateRenderBuffer("cbuffer://batcher2d", genericDynBufferDesc);
    shaderMan->CreateProgram("shader://batcher", Renderer2D::m_Batcher2DLayout, "shader:batch.v", "shader:batch.p");

    // Font constant buffer
    genericDynBufferDesc.DataLen = sizeof(FontRenderBuffer);
    shaderMan->CreateRenderBuffer("cbuffer://font", genericDynBufferDesc);
    fontShader = shaderMan->CreateProgram("shader://font", Renderer2D::m_Batcher2DLayout, "shader:font.v", "shader:font.p");

    //* Fonts *//
    FontDesc desc;
    desc.SizePX = 40;
    pFont = resourceMan->LoadResource<Font>("font://font", "font:font", &desc);
}

void GameApp::OnResolutionChanged(u32 width, u32 height)
{
    ZoneScoped;

    Entity camera2D = m_pCurrentScene->GetEntity("entity://camera2d");
    Entity camera3D = m_pCurrentScene->GetEntity("entity://camera3d");
    auto &camera2DComp = camera2D.GetComponent<Component::Camera2DController>();
    auto &camera3DComp = camera3D.GetComponent<Component::Camera3DController>();

    camera2DComp.m_Handle.SetSize(glm::vec2(width, height));
    camera3DComp.m_Handle.SetSize(glm::vec2(width, height));
}