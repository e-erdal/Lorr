//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "BaseApp.hh"

#include "Engine/Audio/AudioSystem.hh"
#include "Engine/Core/Window/IWindow.hh"
#include "Engine/Debug/ImGuiHandler.hh"
#include "Engine/Graphics/Camera2D.hh"
#include "Engine/Graphics/Renderer/Renderer2D.hh"
#include "Engine/Graphics/Renderer/VertexBatcher.hh"
#include "Engine/Managers/ResourceManager.hh"

#include <box2d/box2d.h>

namespace Lorr
{
    class Engine
    {
    public:
        ~Engine();

        bool Init(ApplicationDesc const &Description);
        void Run();

        // Graphics
        void BeginFrame();
        void EndFrame();

        // Logic
        void Tick(float fDelta);

    public:
        PlatformWindow *GetWindow() const
        {
            return m_pWindow;
        }

        Renderer2D *GetRenderer() const
        {
            return m_pRenderer;
        }

        Camera2D *GetCamera() const
        {
            return m_pCamera;
        }

        ResourceManager *GetResourceMan() const
        {
            return m_pResourceMan;
        }

        AudioSystem *GetAudioSys() const
        {
            return m_pAudioSystem;
        }

        VertexBatcher *GetBatcher() const
        {
            return m_pBatcher;
        }

        b2World *GetPhyWorld()
        {
            return m_World;
        }

    private:
        // Graphics
        PlatformWindow *m_pWindow = new PlatformWindow;
        Renderer2D *m_pRenderer = new Renderer2D;
        VertexBatcher *m_pBatcher = new VertexBatcher;
        Camera2D *m_pCamera = new Camera2D;
        ImGuiHandler *m_pImGui = new ImGuiHandler;
        ResourceManager *m_pResourceMan = new ResourceManager;
        AudioSystem *m_pAudioSystem = new AudioSystem;

        b2World *m_World = 0;
    };

    inline Engine *GetEngine()
    {
        return GetApp()->GetEngine();
    }

}  // namespace Lorr