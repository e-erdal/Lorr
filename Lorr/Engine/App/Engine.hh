//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "BaseApp.hh"

#include "Engine/Audio/AudioSystem.hh"

#include "Engine/Core/Window/BaseWindow.hh"

#include "Engine/Debug/ImGuiHandler.hh"

#include "Engine/Graphics/Camera2D.hh"
#include "Engine/Graphics/Camera3D.hh"
#include "Engine/Graphics/Common/BaseRenderer.hh"
#include "Engine/Graphics/Renderer2D.hh"

#include "Engine/Managers/InputManager.hh"
#include "Engine/Managers/ShaderManager.hh"
#include "Engine/Managers/ResourceManager.hh"

namespace lr
{
    class Engine
    {
    public:
        bool Init(ApplicationDesc const &Description);
        void Run();

        // Graphics
        void BeginFrame();
        void EndFrame();

        // Logic
        void Tick(float fDelta);

    public:
        // clang-format off
        PlatformWindow *GetWindow()          { return &m_Window; }
        BaseRenderer *GetRenderer()          { return m_pRenderer; }
        Renderer2D *GetRenderer2D()          { return &m_Renderer2D; }
        InputManager *GetInputMan()          { return &m_InputMan; }
        ResourceManager *GetResourceMan()    { return &m_ResourceMan; }
        ShaderManager *GetShaderMan()        { return &m_ShaderMan; }
        AudioSystem *GetAudioSys()           { return &m_AudioSystem; }
        // clang-format on

    private:
        // Graphics
        PlatformWindow m_Window;
        BaseRenderer *m_pRenderer = nullptr;
        Renderer2D m_Renderer2D;

        ImGuiHandler m_ImGui;

        InputManager m_InputMan;
        ResourceManager m_ResourceMan;
        ShaderManager m_ShaderMan;

        AudioSystem m_AudioSystem;
    };

    inline Engine *GetEngine()
    {
        return GetApp()->GetEngine();
    }

}  // namespace lr