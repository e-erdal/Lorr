//
// Created on Thursday 9th September 2021 by e-erdal
//

#pragma once

#include "Engine/Core/Window/BaseWindow.hh"
#include "Engine/ECS/Scene.hh"
#include "Engine/Graphics/Types.hh"

namespace lr
{
    constexpr std::string_view kSaveFileName = "state.lr";
    struct BaseAppSave
    {
        u32 m_LastSaveDate = 0;  // Unix timestamp

        /// Graphics
        u8 m_CurrentMonitor = 0;
        u16 m_RefreshRate = 0;
        u16 m_WindowPosX = 0;
        u16 m_WindowPosY = 0;
        WindowFlags m_WindowFlags;
        RendererType m_SelectedRenderer;
    };

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

    public:
        void SetCurrentScene(Scene *pScene);
        Scene *GetActiveScene() const
        {
            return m_pCurrentScene;
        }

    protected:
        Engine *m_pEngine = 0;
        Scene *m_pCurrentScene = 0;

        bool m_ConsoleApp = false;
    };

    extern BaseApp *GetApp();
}  // namespace lr