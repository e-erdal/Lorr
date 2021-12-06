#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/CameraComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Graphics/Font.hh"
#include "Engine/Graphics/Renderer2D.hh"

#include "Engine/Model/Model.hh"

#include "Engine/Job/Worker.hh"

using namespace Lorr;

TextureHandle texture;
Font *pFont;
ShaderProgram *fontShader;

Entity textEntity;

void GameApp::Init()
{
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
    pRenderer->CreateTarget("renderer://postprocess", width, height, 0);
    pRenderer->CreateTarget("renderer://shadowmap", 512, 512, 0);

    //** Init entities **//
    Entity camera3D = m_pCurrentScene->CreateEntity("entity://camera3d");
    camera3D.AttachCamera3D(glm::vec3(0, 0, -5), glm::vec2(width, height), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 60.f, 0.1f, 10000.f);

    Entity camera2D = m_pCurrentScene->CreateEntity("entity://camera2d");
    camera2D.AttachCamera2D(glm::vec2(0, 0), glm::vec2(width, height));
    {
        textEntity = m_pCurrentScene->CreateEntity("test");
        auto &transformComp = textEntity.AddComponent<Component::Transform>(glm::vec3(width / 2, 0, 1), glm::vec3(50, 50, 1));
        auto &textComp = textEntity.AddComponent<Component::Text>(pFont, TextAlignment::Left, "Left\nAligned\nText");
        transformComp.Position.x -= (textComp.m_Size.x / 2) * transformComp.Size.x;
    }
    {
        textEntity = m_pCurrentScene->CreateEntity("test2");
        auto &transformComp = textEntity.AddComponent<Component::Transform>(glm::vec3(width / 2, 200, 1), glm::vec3(50, 50, 1));
        auto &textComp = textEntity.AddComponent<Component::Text>(pFont, TextAlignment::Middle, "Center\nAligned\nText");
        transformComp.Position.x -= (textComp.m_Size.x / 2) * transformComp.Size.x;
    }
    {
        textEntity = m_pCurrentScene->CreateEntity("test3");
        auto &transformComp = textEntity.AddComponent<Component::Transform>(glm::vec3(width / 2, 400, 1), glm::vec3(50, 50, 1));
        auto &textComp = textEntity.AddComponent<Component::Text>(pFont, TextAlignment::Right, "Right\nAligned\nText");
        transformComp.Position.x -= (textComp.m_Size.x / 2) * transformComp.Size.x;
    }
    {
        textEntity = m_pCurrentScene->CreateEntity("test3");
        auto &transformComp = textEntity.AddComponent<Component::Transform>(glm::vec3(width / 2, 600, 1), glm::vec3(50, 50, 1));
        auto &textComp = textEntity.AddComponent<Component::Text>(pFont, TextAlignment::Left, "Tabs \tålşö wıth\nütf8\tğ Ğ");
        transformComp.Position.x -= (textComp.m_Size.x / 2) * transformComp.Size.x;
    }
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();
    BaseRenderer *pRenderer = GetEngine()->GetRenderer();
    VertexBatcher *pBatcher = GetEngine()->GetBatcher();
    ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

    pRenderer->UseConstantBuffer(pShaderMan->GetRenderBuffer("cbuffer://font"), RenderBufferTarget::Pixel, 0);
    Renderer2D::FullscreenQuad(pFont->GetTexture(), pShaderMan->GetProgram("shader://font"));

    ImGui::Begin("GameApp", nullptr);
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    glm::vec3 &size = textEntity.GetComponent<Component::Transform>().Size;
    ImGui::SliderFloat("Scale", &size.x, 0, 2000);
    size.y = size.x;
    ImGui::End();
}

void GameApp::LoadResources()
{
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
    shaderMan->CreateProgram("shader://model", Mesh::m_Layout, "shaders/modelv.lr", "shaders/modelp.lr");

    // VertexBatcher constant buffer
    genericDynBufferDesc.DataLen = sizeof(BatcherRenderBuffer);
    shaderMan->CreateRenderBuffer("cbuffer://batcher", genericDynBufferDesc);
    shaderMan->CreateProgram("shader://batcher", VertexBatcher::m_Layout, "shaders/batchv.lr", "shaders/batchp.lr");

    // Font constant buffer
    genericDynBufferDesc.DataLen = sizeof(FontRenderBuffer);
    shaderMan->CreateRenderBuffer("cbuffer://font", genericDynBufferDesc);
    fontShader = shaderMan->CreateProgram("shader://font", VertexBatcher::m_Layout, "shaders/fontv.lr", "shaders/fontp.lr");

    //* Fonts *//
    FontDesc desc;
    desc.SizePX = 40;
    pFont = resourceMan->LoadResource<Font>("font://font", "font.lr", &desc);
}

void GameApp::OnResolutionChanged(u32 width, u32 height)
{
    Entity camera2D = m_pCurrentScene->GetEntity("entity://camera2d");
    Entity camera3D = m_pCurrentScene->GetEntity("entity://camera3d");
    auto &camera2DComp = camera2D.GetComponent<Component::Camera2DController>();
    auto &camera3DComp = camera3D.GetComponent<Component::Camera3DController>();

    camera2DComp.m_Handle.SetSize(glm::vec2(width, height));
    camera3DComp.m_Handle.SetSize(glm::vec2(width, height));
}