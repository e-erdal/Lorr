#include "Engine.hh"

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"
#include "Engine/Graphics/D3D12/D3D12Renderer.hh"

#include "Engine/Graphics/Renderer2D.hh"

#include "Engine/Scripting/Lua.hh"

#include "Engine/Model/RT/EmbreeAPI.hh"

namespace lr
{
    BufferStreamMemoyWatcher *g_pBSWatcher;

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
        m_Window.Init(description.Title, 0, description.Width, description.Height, description.Flags);

        //* High priority stuff
        m_ResourceMan.Init();

        //* Graphics
        m_ShaderMan.Init();
        m_pRenderer->Init(GetWindow(), m_Window.GetWidth(), m_Window.GetHeight());
        m_ImGui.Init(this);
        m_Renderer2D.Init();

        //* Audio system
        m_AudioSystem.Init();

        /// Scripting ///
        Lua::Init();

        /// RAY TRACING ///
        RT::Init();

        return true;
    }

    void Engine::BeginFrame()
    {
        ZoneScoped;
        
        m_pRenderer->BeginFrame();
        m_ImGui.BeginFrame();
    }

    void Engine::EndFrame()
    {
        ZoneScoped;

        m_ImGui.EndFrame();
        m_pRenderer->Frame();
    }

    void Engine::Tick(float deltaTime)
    {
        ZoneScoped;
    }

}  // namespace lr