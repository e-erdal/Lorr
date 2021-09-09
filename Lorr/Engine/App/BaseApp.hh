//
// Created on Thursday 9th September 2021 by e-erdal
//

#pragma once

#include "Engine/Core/Window/IWindow.hh"

namespace Lorr
{
    struct ApplicationDesc
    {
        std::string Title = "Lorr";
        std::string Icon = "";

        WindowFlags Flags = WindowFlags::None;

        uint32_t Width = 0;
        uint32_t Height = 0;
    };

    class Engine;
    class BaseApp
    {
    public:
        BaseApp();

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
        Engine *m_pEngine = 0;
    };

    extern BaseApp *GetApp();
}  // namespace Lorr