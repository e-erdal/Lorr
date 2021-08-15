#include "GameApp.hh"

#include "Core/Stream/FileStream.hh"

void GameApp::Init()
{
    using namespace Lorr;

    FileStream file( "test.txt", true );

    file.WriteString( "Holy shit\n" );
    file.WriteString( "Holy shit" );
    file.WriteString( "Holy shit" );
    file.WriteString( "Holy shit" );
    file.WriteString( "Holy shit" );
    file.WriteString( "Holy shit" );

    file.Close();
}

void GameApp::Tick( float fDelta )
{
}

void GameApp::Draw()
{
}