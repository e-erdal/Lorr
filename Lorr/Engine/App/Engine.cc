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

        uint32_t v = /*0xfabff000*/ PACK_VERSION(13, 1, 62);
        printf("%x\n", v);
        uint32_t major = 0;
        uint32_t minor = 0;
        uint32_t build = 0;
        UNPACK_VERSION(v, major, minor, build);
        LOG_INFO("Version: v{}.{}.{}", major, minor, build);

        m_pWindow->Init(Description.Title, Description.Width, Description.Height, Description.Flags);

        m_pRenderer->Init(m_pWindow);
        m_pCamera->Init({ 0, 0 }, { m_pWindow->GetWidth(), m_pWindow->GetHeight() });
        m_pImGui->Init(this);

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;

        m_pCamera->SetUniformTransform();

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