#include "GameApp.hh"

#include "Core/Buffer/Buffer.hh"

void GameApp::Init()
{
    Lorr::Buffer b;
    b.AssignString( "Hello this is first string using my first buffer implementation." );

    for ( int i = 0; i < b.GetSize(); i++ ) printf( "%x ", b.GetData()[i] );
}

void GameApp::Tick( float fDelta )
{
}

void GameApp::Draw()
{
}