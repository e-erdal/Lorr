//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Core/Debug/ImGuiHandler.hh"
#include "Core/Graphics/API/D3D11API.hh"
#include "Core/Graphics/Camera.hh"
#include "Core/Graphics/Window.hh"

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

        bool Init( ApplicationDesc const &Description );
        void Run();

    private:
        // Graphics
        void BeginFrame();
        void EndFrame();

        // Logic
        void Tick( float fDelta );

    public:  // OOP :)
        Window *GetWindow() const
        {
            return m_pWindow;
        }

        D3D11API *GetAPI() const
        {
            return m_pAPI;
        }

        Camera *GetCamera() const
        {
            return m_pCamera;
        }

    private:
        // Graphics
        Window *m_pWindow = new Window;
        D3D11API *m_pAPI = new D3D11API;
        Camera *m_pCamera = new Camera;
        ImGuiHandler *m_pImGui = new ImGuiHandler;

        // Resources
    };

    class BaseApp
    {
    public:
        ~BaseApp();

        void Start( ApplicationDesc const &Description );
        void Run();

    public:
        Engine *GetEngine() const
        {
            return m_pEngine;
        }

        virtual void Init() = 0;

        virtual void Tick( float fDelta ) = 0;
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