#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/Font.hh"

#include "Engine/Model/Model.hh"

using namespace Lorr;

void GameApp::Init()
{
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("game://default-scene");

    glm::mat4 mat = glm::mat4(1.f);
    GetEngine()->GetShaderMan()->CreateProgram("game://model", kMeshLayout, "modelv.lr", "modelp.lr");
    GetEngine()->GetShaderMan()->CreateCBuffer("game://model", mat, true);

    Entity modelEntity = m_pCurrentScene->CreateEntity("test");
    modelEntity.AddComponent<Component::Transform>(glm::vec3(), glm::vec3());
    Model &model = modelEntity.AddComponent<Model>();
    model.Init("test.obj");
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();
}
