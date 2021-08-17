#include "GameApp.hh"

#include "Growtopia/RTTexture.hh"

void GameApp::Init()
{
    using namespace Lorr;

    RTTEX::ToTexture( "gt://game-tiles_page1", "tiles_page1.rttex" );
}

void GameApp::Tick( float fDelta )
{
}

void GameApp::Draw()
{
}