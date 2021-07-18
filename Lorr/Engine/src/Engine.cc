#include "Engine.hh"

#include "Core/Utils/Math.hh"
#include "Core/Utils/Timer.hh"

namespace Lorr
{
    Engine::~Engine()
    {
        delete m_pWindow;
        delete m_pAPI;
        delete m_pImGui;
        delete m_pCamera;
    }

    bool Engine::Init( ApplicationDesc const &Description )
    {
        Console::Init();
        Console::Info( "Initializing Lorr..." );

        m_pWindow->Init( Description.sTitle, Description.uWidth, Description.uHeight, WindowFlags::Resizable | WindowFlags::Centered );
        m_pAPI->Init( m_pWindow, Description.uWidth, Description.uHeight );
        m_pCamera->Init( { 0, 0, 10 }, { 0, 1, 0 }, { Description.uWidth, Description.uHeight }, 60.f, 0.0001f, 10000.f );

        m_pImGui->Init( this );

        return true;
    }

    void Engine::BeginFrame()
    {
        m_pAPI->SetClearColor( { 0.1, 0.1, 0.1, 1 } );
        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        m_pImGui->EndFrame();
        m_pAPI->Frame( 0 );
    }

    BaseApp::~BaseApp()
    {
    }

    void BaseApp::Start( ApplicationDesc const &Description )
    {
        m_pEngine->Init( Description );
    }

    void BaseApp::Run()
    {
        Window *pWindow = m_pEngine->GetWindow();

        Timer timer;
        while ( !pWindow->ShouldClose() )
        {
            auto elapsed = timer.elapsed();
            timer.reset();

            Tick( elapsed );

            m_pEngine->BeginFrame();
            Draw();
            m_pEngine->EndFrame();

            pWindow->Poll();
        }
    }
}  // namespace Lorr