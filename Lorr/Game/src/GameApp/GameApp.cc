#include "GameApp.hh"

#include "Core/Stream/BufferStream.hh"
#include "Core/Stream/FileStream.hh"

#include "Growtopia/ItemInfoManager.hh"

void GameApp::Init()
{
    using namespace Lorr;

    // RTTEX::ToTexture( "gt://game-tiles_page1", "tiles_page1.rttex" );

    // FileStream file( "items.dat", false );

    // if ( !file.IsOK() )
    // {
    //     Console::Fatal( "Failed to load file 'items.dat'!" );
    //     return;
    // }

    // uint8_t *pData = file.ReadAll<uint8_t>();
    // size_t dataSize = file.Size();

    // BufferStream buffer( pData, dataSize );

    // ItemInfoManager iim;
    // iim.Unpack( buffer );
}

void GameApp::Tick(float fDelta)
{
}

void GameApp::Draw()
{
}