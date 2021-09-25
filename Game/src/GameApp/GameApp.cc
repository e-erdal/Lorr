#include "GameApp.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

Audio *audio = new Audio;
AudioChannel *channel = new AudioChannel;

void GameApp::Init()
{
    AudioData data;
    GetEngine()->GetResourceMan()->ImportAudioData("test.lr", data);

    GetEngine()->GetAudioSys()->CreateChannel("", channel);
    GetEngine()->GetAudioSys()->LoadAudio("", audio, channel, &data);
    audio->Play();
}

void GameApp::Tick(float fDelta)
{
}

void GameApp::Draw()
{
}