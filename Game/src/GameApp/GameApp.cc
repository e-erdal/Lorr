#include "GameApp.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

void GameApp::Init()
{
}

void GameApp::Tick(float fDelta)
{
}

void GameApp::Draw()
{
    ImGui::Begin("asd", nullptr);
    ImGui::End();

    glm::mat4 transform = glm::translate(glm::mat4(1.f), { 0, 0, 1 }) * glm::scale(glm::mat4(1.f), { 100, 100, 1 });

    GetEngine()->GetBatcher()->Begin();
    GetEngine()->GetBatcher()->PushRect(0, transform, { 1, 1, 1, 1 });
    GetEngine()->GetBatcher()->End();
}