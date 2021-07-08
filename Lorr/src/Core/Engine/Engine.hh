//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/API/D3D11API.hh"
#include "Core/Graphics/Camera.hh"
#include "Core/Graphics/Window.hh"

struct EngineDesc
{
    std::string sTitle = "Lorr";
    uint32_t uWidth = 0;
    uint32_t uHeight = 0;
};

class Engine
{
public:
    Engine( EngineDesc const &Description );
    ~Engine();

    void Run();

private:
    bool PreInit();

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
    D3D11API *m_pAPI = 0;
    Camera *m_pCamera = 0;

    // Resources
};

extern Engine *GetEngine();