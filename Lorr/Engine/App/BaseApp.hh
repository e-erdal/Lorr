//
// Created on Thursday 9th September 2021 by e-erdal
//

#pragma once

#include "Engine/Core/Window/IWindow.hh"
#include "Engine/ECS/Scene.hh"

namespace Lorr
{
    struct ApplicationDesc
    {
        std::string Title = "Lorr";
        std::string Icon = "";

        WindowFlags Flags = WindowFlags::None;

        u32 Width = 0;
        u32 Height = 0;

        bool ConsoleApp = false;
    };

    class Engine;
    class BaseApp
    {
    public:
        BaseApp();

        ~BaseApp();

        void Start(ApplicationDesc const &description);
        void Run();

    public:
        Engine *GetEngine() const
        {
            return m_pEngine;
        }

        virtual void Init() = 0;

        virtual void Tick(float deltaTime) = 0;
        virtual void Draw() = 0;

        void SetCurrentScene(Scene *pScene);

    protected:
        Engine *m_pEngine = 0;
        Scene *m_pCurrentScene = 0;
        
        bool m_ConsoleApp = false;
    };

    extern BaseApp *GetApp();
}  // namespace Lorr