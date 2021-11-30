#include "BaseApp.hh"

#include "Engine.hh"

#include "Engine/Utils/FPSLimiter.hh"

namespace Lorr
{
    BaseApp::BaseApp() : m_pEngine(new Engine)
    {
    }

    BaseApp::~BaseApp()
    {
        ZoneScoped;
    }

    void BaseApp::Start(ApplicationDesc const &description)
    {
        ZoneScoped;

        m_pEngine->Init(description);
        m_ConsoleApp = description.ConsoleApp;

        Init();
    }

    void BaseApp::Run()
    {
        if (m_ConsoleApp) return;

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

    void BaseApp::SetCurrentScene(Scene *pScene)
    {
        m_pCurrentScene = pScene;
    }
}  // namespace Lorr