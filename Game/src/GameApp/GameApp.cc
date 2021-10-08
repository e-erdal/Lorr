#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/Font.hh"

using namespace Lorr;

Font *font = new Font;

void GameApp::Init()
{
    m_pCurrentScene->Init("game://default-scene");

    font = GetEngine()->GetResourceMan()->LoadFont("", "font.ttf", 18);

    Entity test2 = m_pCurrentScene->CreateEntity("ase");
    test2.AddComponent<Component::Transform>(glm::vec3(0, 10, 3), glm::vec3(50, 50, 1));
    test2.AddComponent<Component::Renderable>(true);
    ECSUtils::AttachDynamicBox(test2, 1, 0.5);

    Entity test3 = m_pCurrentScene->CreateEntity("ase");
    test3.AddComponent<Component::Transform>(glm::vec3(0, 300, 3), glm::vec3(350, 50, 1), 1.f);
    test3.AddComponent<Component::Renderable>(true, glm::ivec4(255, 0, 0, 255));
    ECSUtils::AttachStaticBox(test3, 0);

    Entity test4 = m_pCurrentScene->CreateEntity("ase");
    test4.AddComponent<Component::Transform>(glm::vec3(650, 530, 3), glm::vec3(350, 50, 1), 0.f);
    test4.AddComponent<Component::Renderable>(true, glm::ivec4(255, 0, 0, 255));
    ECSUtils::AttachStaticBox(test4, 0);

    m_pCurrentScene->SortAllByDepth();
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();
}