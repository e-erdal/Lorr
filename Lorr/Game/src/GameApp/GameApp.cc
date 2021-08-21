#include "GameApp.hh"

#include "Core/Stream/BufferStream.hh"
#include "Core/Stream/FileStream.hh"

#include "Growtopia/RTTexture.hh"

void GameApp::Init()
{
    using namespace Lorr;

    TEXTURE2D_DESC texDesc;
    texDesc.Filters = TEXTURE_FILTER_NEAREST;
    texDesc.Path = "tiles1.png";

    Texture2D *tex = Texture2D::Create("texture://tiles1", &texDesc);
    
    
}

void GameApp::Tick(float fDelta)
{
}

void GameApp::Draw()
{
}