//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Graphics/Camera.hh"
#include "Graphics/Window.hh"

struct AppDesc
{
    std::string sTitle = "GLCraft";
    uint32_t uWidth = 0;
    uint32_t uHeight = 0;
};

class App
{
public:
    App( AppDesc const &Description );
    ~App();

private:
    bool PreInit();
    void Run();

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

    Camera *GetCamera() const
    {
        return m_pCamera;
    }

private:
    // Graphics
    Window *m_pWindow = 0;
    Camera *m_pCamera = 0;
};