#include "Engine.hh"

#include "Core/Utils/Math.hh"

Engine::Engine( EngineDesc const &Description )
{
    if ( PreInit() )
    {
        m_pWindow = new Window( Description.sTitle, Description.uWidth, Description.uHeight, WindowFlags::Fullscreen );
        m_pAPI->Init( m_pWindow, Description.uWidth, Description.uHeight );

        m_pCamera = new Camera( { 0, 0, 10 }, { 0, 1, 0 }, { Description.uWidth, Description.uHeight }, 60.f, 0.0001f, 10000.f );
    }
}

Engine::~Engine()
{
    delete m_pWindow;
}

bool Engine::PreInit()
{
    Console::Init();
    Console::Info( "Initializing Lorr..." );

    m_pAPI = new D3D11API();
    return true;
}

void Engine::Run()
{
    while ( !m_pWindow->ShouldClose() )
    {
        BeginFrame();

        EndFrame();

        m_pWindow->Poll();
    }
}

void Engine::BeginFrame()
{
    m_pAPI->SetClearColor( { 0.2, 0.2, 0.2, 1 } );
}

void Engine::EndFrame()
{
    m_pAPI->Frame( 0 );
}