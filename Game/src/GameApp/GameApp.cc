#include "GameApp.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

Audio *audio = new Audio;
AudioChannel *channel = new AudioChannel;

void GameApp::Init()
{
    AudioData data;
    GetEngine()->GetResourceMan()->ImportAudioData("theme.lr", data);

    GetEngine()->GetAudioSys()->CreateChannel("", channel);
    GetEngine()->GetAudioSys()->LoadAudio("", audio, channel, &data);
    audio->Play();
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