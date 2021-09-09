#include "BaseApp.hh"

#include "Engine.hh"

namespace Lorr
{
    BaseApp::BaseApp() : m_pEngine(new Engine)
    {
    }

    BaseApp::~BaseApp()
    {
        ZoneScoped;
    }

    void BaseApp::Start(ApplicationDesc const &Description)
    {
        ZoneScoped;

        m_pEngine->Init(Description);

        Init();
    }

    void BaseApp::Run()
    {
        ZoneScoped;

        PlatformWindow *pWindow = m_pEngine->GetWindow();

        Timer timer;

        while (!pWindow->ShouldClose())
        {
            auto elapsed = timer.elapsed();
            timer.reset();

            m_pEngine->Tick(elapsed);

            Tick(elapsed);

            m_pEngine->BeginFrame();
            Draw();
            m_pEngine->EndFrame();

            pWindow->Poll();

            FrameMark;
        }
    }
}  // namespace Lorr