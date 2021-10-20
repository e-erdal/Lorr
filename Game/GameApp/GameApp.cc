#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"

#include "Engine/Graphics/Font.hh"

#include "Engine/Model/Model.hh"

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

void GameApp::Init()
{
    GetEngine()->GetWindow()->OnSetMouseState.connect<MouseDown>();
    GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();

    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("game://default-scene");

    GetEngine()->GetRenderer()->CreateTarget("renderer://test", GetEngine()->GetWindow()->GetWidth(), GetEngine()->GetWindow()->GetHeight());

    glm::mat4 mat = {};
    GetEngine()->GetShaderMan()->CreateProgram("game://model", kMeshLayout, "modelv.lr", "modelp.lr");
    GetEngine()->GetShaderMan()->CreateCBuffer("game://model", mat, true);

    Entity modelEntity = m_pCurrentScene->CreateEntity("test");
    modelEntity.AddComponent<Component::Transform>(glm::vec3(), glm::vec3());
    Model &model = modelEntity.AddComponent<Model>();

    // model.Init("sponza.obj");
    model.AddSphere(100, 128, GetEngine()->GetRenderer()->GetPlaceholder());
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
    GetEngine()->GetCamera3D()->Update(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();

    ImGui::Begin("GameApp", nullptr);
    ImGui::Text("Max FPS set to 200.");
    ImGui::Image((ImTextureID)GetEngine()->GetRenderer()->GetTargetTexture("renderer://test"),
                 { GetEngine()->GetWindow()->GetWidth() / 2.f, GetEngine()->GetWindow()->GetHeight() / 2.f });
    ImGui::End();
}
