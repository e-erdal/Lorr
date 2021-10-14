#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/Font.hh"

using namespace Lorr;

void GameApp::Init()
{
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("game://default-scene");

    FontDesc desc;
    desc.SizePX = 18;

    Font *pFont = GetEngine()->GetResourceMan()->LoadResource<Font>("game://font-18", "font.lr", &desc);
    // Entity ent = m_pCurrentScene->CreateEntity("tes");
    // ent.AttachText("game://font-18",
    //                "Lorr is a game engine developed by Emirhan Erdal.\n"
    //                "Legal notices of third party libraries used in Lorr.\n\n\n"

    //                "bgfx • Copyright © 2010 - 2021 Branimir Karadzic\n"
    //                "imgui • Copyright © 2014 - 2021 Omar Cornut\n"
    //                "OpenAL • Copyright © 2007 - 2021 Free Software Foundation, Inc.\n\n\n"
    //                "Click here for details",
    //                { 100, 100, 1 });

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
