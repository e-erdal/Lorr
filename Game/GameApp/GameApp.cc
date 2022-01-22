#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/CameraComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Graphics/Font.hh"
#include "Engine/Graphics/Renderer2D.hh"

#include "Engine/Terrain/Terrain.hh"

using namespace lr;

Terrain terrain;

TextureHandle texture;
Font *pFont;
ShaderProgram *fontShader;

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

    Entity terrainEntity = m_pCurrentScene->CreateEntity("entity://terrain");
    terrain = terrainEntity.AddComponent<Terrain>();

    terrain.Init(TerrainType::Detailed, 10, 10);
}

void GameApp::Tick(float deltaTime)
{
    ZoneScoped;

    m_pCurrentScene->Tick(deltaTime);

    terrain.Update(deltaTime);
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

    if (ImGui::Button("Create Ray"))
    {
        terrain.PickMouse(1280 / 2.0, 720 / 2.0);
    }

    if (ImGui::Checkbox("Wireframe", &wireframe))
    {
        pRenderer->SetWireframeState(wireframe);
    }

    ImGui::End();

    terrain.Render();
}

void GameApp::LoadResources()
{
    ZoneScoped;

    ResourceManager *pResourceMan = GetEngine()->GetResourceMan();

    //* Fonts *//
    FontDesc desc;
    desc.SizePX = 40;
    pFont = pResourceMan->LoadResource<Font>("font://font", "font:font", &desc);
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