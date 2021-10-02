#include "Engine.hh"

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

    bool Engine::Init(ApplicationDesc const &Description)
    {
        ZoneScoped;

        Console::Init();
        LOG_INFO("Initializing Lorr...");

        //* Core features
        m_pWindow->Init(Description.Title, Description.Width, Description.Height, Description.Flags);

        //* High priority stuff
        m_pResourceMan->Init();

        //* Graphics
        m_pRenderer->Init(m_pWindow);
        m_pBatcher->Init();
        m_pCamera->Init({ 0, 0 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() });
        m_pImGui->Init(this);

        //* Audio system
        m_pAudioSystem->Init();

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;

        m_pRenderer->SetViewTransform(0, m_pCamera->GetProjection(), m_pCamera->GetView());

        m_pRenderer->BeginFrame();
        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_pImGui->EndFrame();
        m_pRenderer->EndFrame();
    }

    void Engine::Tick(float fDelta)
    {
        ZoneScoped;
    }

}  // namespace Lorr

#if TRACY_ENABLE
void *operator new(size_t s)
{
    void *p = malloc(s);

    TracySecureAlloc(p, s);

    return p;
}

void operator delete(void *p) noexcept
{
    TracySecureFree(p);

    free(p);
}
#endif