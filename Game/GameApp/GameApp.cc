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

void GameApp::Init()
{
    //* Init signals
    GetEngine()->GetWindow()->OnSetMouseState.connect<MouseDown>();
    GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();

    //* Required vars
    IRenderer *pRenderer = GetEngine()->GetRenderer();
    u32 width = GetEngine()->GetWindow()->GetWidth();
    u32 height = GetEngine()->GetWindow()->GetHeight();
    glm::mat4 mat = {};

    //* Scene initialization
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("scene://default-scene");

    //* Preload resources
    FontDesc desc;
    desc.SizePX = 32;
    Font *pFont = GetEngine()->GetResourceMan()->LoadResource<Font>("font://font", "font.lr", &desc);

    // Post-process target
    pRenderer->CreateTarget("renderer://postprocess", width, height, 0, 6);
    pRenderer->CreateTarget("renderer://shadowmap", 512, 512, 0);

    // Load shaders
    GetEngine()->GetShaderMan()->CreateProgram("shader://model", kMeshLayout, "shaders/modelv.lr", "shaders/modelp.lr");
    GetEngine()->GetShaderMan()->CreateProgram("shader://batcher", VertexBatcher::m_Layout, "shaders/batchv.lr", "shaders/batchp.lr");

    RenderBufferDesc modelCBuf;
    modelCBuf.DataLen = sizeof(mat);
    modelCBuf.Type = RenderBufferType::Constant;
    modelCBuf.Usage = RenderBufferUsage::Dynamic;
    modelCBuf.MemFlags = RenderBufferMemoryFlags::Access_CPUW;
    GetEngine()->GetShaderMan()->CreateRenderBuffer("cbuffer://model", modelCBuf);

    modelCBuf.DataLen = sizeof(mat);
    GetEngine()->GetShaderMan()->CreateRenderBuffer("cbuffer://batcher", modelCBuf);

    //** Init entities **//
    // Create entities
    Entity modelEntity = m_pCurrentScene->CreateEntity("test");
    modelEntity.AddComponent<Component::Transform>(glm::vec3(), glm::vec3());
    Model &model = modelEntity.AddComponent<Model>();

    ModelDesc modelDesc;
    modelDesc.Dynamic = false;
    ModelData modelData;
    GetEngine()->GetResourceMan()->ImportResource(ResourceType::Model, "models/island.lr", modelData);
    model.Init("resource://teapot", &modelDesc, &modelData);
    // model.Init("sponza.obj");
    // model.AddSphere(100, 128, pRenderer->GetPlaceholder());
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
    pRenderer->PollPostProcess();

    Renderer2D::FullscreenQuad(pRenderer->GetTargetTexture("renderer://postprocess"), 0);

    ImGui::Begin("GameApp", nullptr);
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    TextureHandle shadowMap = pRenderer->GetTargetTexture("renderer://shadowmap");
    ImGui::Image(shadowMap, { (float)shadowMap->GetWidth(), (float)shadowMap->GetHeight() });
    // ImGui::SliderFloat("LOD", &lensDownsampleData.ScaleBias.z, 0, );
    ImGui::End();
}