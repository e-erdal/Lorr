//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "BaseApp.hh"

#include "Engine/Core/Window/IWindow.hh"
#include "Engine/Debug/ImGuiHandler.hh"
#include "Engine/Graphics/Camera2D.hh"
#include "Engine/Graphics/Renderer/Renderer2D.hh"

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

    private:
        // Graphics
        PlatformWindow *m_pWindow = new PlatformWindow;
        Renderer2D *m_pRenderer = new Renderer2D;
        Camera2D *m_pCamera = new Camera2D;
        ImGuiHandler *m_pImGui = new ImGuiHandler;
    };

    inline Engine *GetEngine()
    {
        return GetApp()->GetEngine();
    }

}  // namespace Lorr