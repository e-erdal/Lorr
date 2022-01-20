#include "Engine.hh"

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"
#include "Engine/Graphics/D3D12/D3D12Renderer.hh"

#include "Engine/Graphics/Renderer2D.hh"

#include "Engine/Scripting/Lua.hh"

namespace lr
{
    BufferStreamMemoyWatcher *g_pBSWatcher;

    Engine::~Engine()
    {
        ZoneScoped;

        delete m_pWindow;
        // delete m_pRenderer;
        delete m_pImGui;
    }

    bool Engine::Init(ApplicationDesc const &description)
    {
        ZoneScoped;

        Console::Init();
        LOG_TRACE("Initializing Lorr...");

        g_pBSWatcher = new BufferStreamMemoyWatcher(false);

        // TODO: Auto select
        m_pRenderer = new D3D11Renderer;

        if (description.ConsoleApp)
        {
            m_pRenderer->Init(0, 0, 0);
            return true;
        }

        //* Core features
        m_pWindow->Init(description.Title, 0, description.Width, description.Height, description.Flags);

        //* High priority stuff
        m_pResourceMan->Init();

        //* Graphics
        m_pShaderMan->Init();
        m_pRenderer->Init(m_pWindow, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        m_pImGui->Init(this);
        m_pRenderer2D->Init();

        //* Audio system
        m_pAudioSystem->Init();

        //* Physics system
        b2Vec2 gravity(0.0f, 9.89f);
        m_World = new b2World(gravity);

        /// Scripting ///
        Lua::Init();

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;
        
        m_pRenderer->BeginFrame();
        m_pImGui->BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_pImGui->EndFrame();
        m_pRenderer->Frame();
    }

    void Engine::Tick(float deltaTime)
    {
        ZoneScoped;
    }

}  // namespace lr