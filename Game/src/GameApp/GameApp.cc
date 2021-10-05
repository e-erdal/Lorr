#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"

#include "Engine/Graphics/Font.hh"

using namespace Lorr;

Audio *audio = new Audio;
Font *font = new Font;

std::vector<CharInfo> chars;

void GameApp::Init()
{
    m_pCurrentScene->Init("game://default-scene");

    Entity test = m_pCurrentScene->CreateEntity("as");
    test.AddComponent<Component::Transform>(glm::vec3{ 200, 200, 1 }, glm::vec3{ 100, 100, 1 });
    test.AddComponent<Component::Renderable>(true);

    Entity test2 = m_pCurrentScene->CreateEntity("ase");
    test2.AddComponent<Component::Transform>(glm::vec3{ 250, 250, 2 }, glm::vec3{ 100, 100, 1 });
    test2.AddComponent<Component::Renderable>(true, glm::ivec4{ 255, 0, 0, 255 });

    m_pCurrentScene->SortAllByDepth();

    AudioData data;
    GetEngine()->GetResourceMan()->ImportAudioData("theme.lr", data);
    GetEngine()->GetAudioSys()->LoadAudio("music://theme", audio, &data, "channel://default");

    audio->Play();

    FontData fdata;
    FileSystem::ReadBinaryFile("font.ttf", fdata.TTFData);

    font->Init("", &fdata, 32);

    glm::vec2 t;
    TextEngine::AlignAll(font, "brö][üğğşşı", chars, t, 0);
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
}

void GameApp::Draw()
{
    ImGui::Begin("asd", nullptr);
    ImGui::End();

    GetEngine()->GetBatcher()->Begin();

    glm::vec2 line = { 10, 10 };
    for (const auto &c : chars)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.f), { 10 + c.Pos.x, 10 + c.Pos.y, 1 }) * glm::scale(glm::mat4(1.f), { c.Size.x, c.Size.y, 1 });
        GetEngine()->GetBatcher()->PushRect(font->GetTexture(), transform, c.UV);
    }

    GetEngine()->GetBatcher()->End();

    m_pCurrentScene->Draw();
}