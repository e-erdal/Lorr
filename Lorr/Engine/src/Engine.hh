//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Core/Window/IWindow.hh"
#include "Debug/ImGuiHandler.hh"
#include "Graphics/Camera2D.hh"
#include "Graphics/Renderer/Renderer2D.hh"

namespace Lorr
{
    struct ApplicationDesc
    {
        std::string sTitle = "Lorr";
        std::string sIcon = "";

        WindowFlags eFlags = WindowFlags::None;

        uint32_t uWidth = 0;
        uint32_t uHeight = 0;
    };

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

    class BaseApp
    {
    public:
        ~BaseApp();

        void Start(ApplicationDesc const &Description);
        void Run();

    public:
        Engine *GetEngine() const
        {
            return m_pEngine;
        }

        virtual void Init() = 0;

        virtual void Tick(float fDelta) = 0;
        virtual void Draw() = 0;

    protected:
        Engine *m_pEngine = new Engine;
    };

    extern BaseApp *GetApp();
    inline Engine *GetEngine()
    {
        return GetApp()->GetEngine();
    }

}  // namespace Lorr