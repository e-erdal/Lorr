#include "GameApp.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

Audio *audio = new Audio;
AudioChannel *channel = new AudioChannel;

void GameApp::Init()
{
    ShaderData data;
    GetEngine()->GetResourceMan()->ImportShaderData("batch.lr", data);
}

void GameApp::Tick(float fDelta)
{
}

void GameApp::Draw()
{
}