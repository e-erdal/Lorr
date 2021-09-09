//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/Systems/Helpers/InputVars.hh"

namespace Lorr
{
    typedef intptr_t PlatformHandle;

    enum class Cursor
    {
        Arrow,
        TextInput,
        ResizeAll,
        ResizeEW,
        ResizeNS,
        ResizeNESW,
        ResizeNWSE,
        Hand,
        NotAllowed,
        Hidden,
    };

    enum class WindowFlags
    {
        None,
        Fullscreen = 1 << 0,
        VSync = 1 << 1,
        Centered = 1 << 2,  // Desktop dist. specific
        Resizable = 1 << 3
    };
    BitFlags(WindowFlags);

    class IWindow
    {
    public:
        virtual ~IWindow(){};

        virtual void Init(const std::string &title, uint32_t width, uint32_t height, WindowFlags flags) = 0;
        virtual void Poll() = 0;

        virtual int GetMonitorWidth() = 0;
        virtual int GetMonitorHeight() = 0;

        virtual void SetCursor(Cursor cursor) = 0;

    public:
        signals::signal<void(Key, ButtonState, KeyMod)> OnSetKeyState;
        signals::signal<void(KeyMod, MouseButton, ButtonState, const glm::ivec2 &)> OnSetMouseState;
        signals::signal<void(glm::ivec2, glm::ivec2)> OnSetMousePosition;
        signals::signal<void()> OnLoseFocus;
        signals::signal<void()> OnGainFocus;
        signals::signal<void(uint32_t, uint32_t)> OnResolutionChanged;

        signals::signal<void(uint32_t, KeyMod)> OnChar;  // Text input

    public:
        virtual bgfx::PlatformData GetPlatformData() = 0;

        uint32_t GetWidth() const
        {
            return m_Width;
        }

        uint32_t GetHeight() const
        {
            return m_Height;
        }

        bool ShouldClose() const
        {
            return m_ShouldClose;
        }

        bool IsFullscreen() const
        {
            return m_IsFullscreen;
        }

        glm::ivec2 GetCursorPos() const
        {
            return m_CursorPos;
        }

    protected:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        bool m_ShouldClose = false;
        bool m_IsFullscreen = false;
        bool m_SizeEnded = true;

        Cursor m_CurrentCursor = Cursor::Arrow;
        glm::ivec2 m_CursorPos{};
    };

}  // namespace Lorr