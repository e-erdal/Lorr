#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/Font.hh"

using namespace Lorr;

Font *font = new Font;

void GameApp::Init()
{
    m_pCurrentScene->Init("game://default-scene");

    font = GetEngine()->GetResourceMan()->LoadFont("", "font.ttf", 18);

    Entity test = m_pCurrentScene->CreateEntity("ase");
    test.AddComponent<Component::Transform>(glm::vec3{ 0, 0, 2 });
    test.AddComponent<Component::Text>(font, "test", 300);

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