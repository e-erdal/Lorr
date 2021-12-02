#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Graphics/Font.hh"
#include "Engine/Graphics/Renderer/Renderer2D.hh"

#include "Engine/Model/Model.hh"

#include "Engine/Job/Worker.hh"

using namespace Lorr;

void KeyDown(Lorr::Key eKey, Lorr::ButtonState eState, Lorr::KeyMod eMod)
{
    Camera3D *pCam = GetEngine()->GetCamera3D();

    if (eState == ButtonState::Pressed)
    {
        switch (eKey)
        {
            case Key::Key_W: pCam->StartMoving(Direction::FORWARD); break;
            case Key::Key_S: pCam->StartMoving(Direction::BACKWARD); break;
            case Key::Key_A: pCam->StartMoving(Direction::LEFT); break;
            case Key::Key_D: pCam->StartMoving(Direction::RIGHT); break;
            case Key::Key_Q: pCam->StartMoving(Direction::UP); break;
            case Key::Key_E: pCam->StartMoving(Direction::DOWN); break;
            default: break;
        }
    }
    else if (eState == ButtonState::Released)
    {
        switch (eKey)
        {
            case Key::Key_W: pCam->StopMoving(Direction::FORWARD); break;
            case Key::Key_S: pCam->StopMoving(Direction::BACKWARD); break;
            case Key::Key_A: pCam->StopMoving(Direction::LEFT); break;
            case Key::Key_D: pCam->StopMoving(Direction::RIGHT); break;
            case Key::Key_Q: pCam->StopMoving(Direction::UP); break;
            case Key::Key_E: pCam->StopMoving(Direction::DOWN); break;
            default: break;
        }
    }
}

bool down = false;
void MouseMove(glm::ivec2, glm::ivec2 offset)
{
    if (!down) return;

    Camera3D *pCam = GetEngine()->GetCamera3D();
    pCam->SetDirection(offset.x, offset.y);
}

void MouseDown(KeyMod, MouseButton, ButtonState state, const glm::ivec2 &)
{
    switch (state)
    {
        case ButtonState::Pressed: down = true; break;
        case ButtonState::Released: down = false; break;
        default: break;
    }
}

TextureHandle texture;
Font *pFont;
ShaderProgram *fontShader;

Entity textEntity;

void GameApp::Init()
{
    LoadResources();

    //* Init signals
    GetEngine()->GetWindow()->OnSetMouseState.connect<MouseDown>();
    GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();

    //* Required vars
    IRenderer *pRenderer = GetEngine()->GetRenderer();
    u32 width = GetEngine()->GetWindow()->GetWidth();
    u32 height = GetEngine()->GetWindow()->GetHeight();

    //* Scene initialization
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("scene://default-scene");

    // Post-process target
    pRenderer->CreateTarget("renderer://postprocess", width, height, 0);
    pRenderer->CreateTarget("renderer://shadowmap", 512, 512, 0);

    //** Init entities **//
    // Create entities

    textEntity = m_pCurrentScene->CreateEntity("test");
    textEntity.AddComponent<Component::Transform>(glm::vec3(0, 0, 1), glm::vec3(30, 30, 1));
    textEntity.AddComponent<Component::Text>(pFont, TextAlignment::Left, "FPS: 500\nFrame(ms): 0...05jjj");
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
    GetEngine()->GetCamera3D()->Update(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();
    IRenderer *pRenderer = GetEngine()->GetRenderer();
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