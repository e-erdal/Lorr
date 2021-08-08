#include "Engine.hh"

#include "Utils/FPSLimiter.hh"
#include "Utils/Math.hh"
#include "Utils/Timer.hh"

namespace Lorr
{
    Engine::~Engine()
    {
        ZoneScoped;

        delete m_pWindow;
        delete m_pRenderer;
        delete m_pImGui;
        delete m_pCamera;
    }

    bool Engine::Init( ApplicationDesc const &Description )
    {
        ZoneScoped;

        Console::Init();
        Console::Info( "Initializing Lorr..." );

        m_pWindow->Init( Description.sTitle, Description.uWidth, Description.uHeight, Description.eFlags );
        m_pRenderer->Init( m_pWindow );
        m_pCamera->Init( { 0, 0 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() } );
        m_pImGui->Init( this );

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;

        // m_pAPI->SetClearColor( { 0.1, 0.1, 0.1, 1 } );
        // m_pAPI->HandlePreFrame();

        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_pImGui->EndFrame();
        // m_pAPI->Frame( 0 );
    }

    void Engine::Tick( float fDelta )
    {
    }

    BaseApp::~BaseApp()
    {
        ZoneScoped;
    }

    void BaseApp::Start( ApplicationDesc const &Description )
    {
        ZoneScoped;

        m_pEngine->Init( Description );

        Init();
    }

    void BaseApp::Run()
    {
        ZoneScoped;

        PlatformWindow *pWindow = m_pEngine->GetWindow();

        Timer timer;

        while ( !pWindow->ShouldClose() )
        {
            auto elapsed = timer.elapsed();
            timer.reset();

            m_pEngine->Tick( elapsed );

            Tick( elapsed );

            m_pEngine->BeginFrame();
            Draw();
            m_pEngine->EndFrame();

            pWindow->Poll();

            FrameMark;
        }
    }
}  // namespace Lorr

#if TRACY_ENABLE
void *operator new( size_t s )
{
    void *p = malloc( s );

    TracySecureAlloc( p, s );

    return p;
}

void operator delete( void *p ) noexcept
{
    TracySecureFree( p );

    free( p );
}
#endif