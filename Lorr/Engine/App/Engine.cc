#include "Engine.hh"

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace Lorr
{
    BufferStreamMemoyWatcher *g_pBSWatcher;

    Engine::~Engine()
    {
        ZoneScoped;

        delete m_pWindow;
        // delete m_pRenderer;
        delete m_pImGui;
        delete m_pCamera2D;
        delete m_pCamera3D;
    }

    bool Engine::Init(ApplicationDesc const &description)
    {
        ZoneScoped;

        Console::Init();
        LOG_INFO("Initializing Lorr...");

        g_pBSWatcher = new BufferStreamMemoyWatcher(false);

        // TODO: Auto select
        m_pRenderer = new D3D11Renderer;

        if (description.ConsoleApp)
        {
            m_pResourceMan->Init();
            m_pAudioSystem->Init();
            m_pRenderer->Init(0, 0, 0);
            return true;
        }

        //* Core features
        m_pWindow->Init(description.Title, description.Width, description.Height, description.Flags);

        //* High priority stuff
        m_pResourceMan->Init();

        //* Graphics
        m_pShaderMan->Init();
        m_pRenderer->Init(m_pWindow, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        m_pBatcher->Init();
        m_pCamera2D->Init({ 0, 0 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() });
        m_pCamera3D->Init({ 0, 0, -5 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() }, { 0, 0, 1 }, { 0, 1, 0 }, 60.f, 0.1f, 10000.f);
        m_pImGui->Init(this);

        //* Audio system
        m_pAudioSystem->Init();

        //* Physics system
        b2Vec2 gravity(0.0f, 9.89f);
        m_World = new b2World(gravity);

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;

        m_pRenderer->HandlePreFrame();
        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_pImGui->EndFrame();
        m_pRenderer->Frame(0);
    }

    void Engine::Tick(float deltaTime)
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