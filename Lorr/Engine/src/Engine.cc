#include "Engine.hh"

#include "Core/Utils/FPSLimiter.hh"
#include "Core/Utils/Math.hh"
#include "Core/Utils/Timer.hh"

namespace Lorr
{
    Engine::~Engine()
    {
        ZoneScoped;

        delete m_pWindow;
        delete m_pAPI;
        delete m_pImGui;
        delete m_pCamera;
    }

    bool Engine::Init( ApplicationDesc const &Description )
    {
        ZoneScoped;

        Console::Init();
        Console::Info( "Initializing Lorr..." );

        m_pWindow->Init( Description.sTitle, Description.uWidth, Description.uHeight, Description.eFlags );
        m_pAPI->Init( m_pWindow, m_pWindow->GetWidth(), m_pWindow->GetHeight() );
        m_pCamera->Init( { 0, 0, 10 }, { 0, 1, 0 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() }, 60.f, 0.0001f, 10000.f );

        m_pImGui->Init( this );

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;

        m_pAPI->SetClearColor( { 0.1, 0.1, 0.1, 1 } );
        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_pImGui->EndFrame();
        m_pAPI->Frame( 0 );
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