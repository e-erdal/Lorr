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

        virtual void Init(const std::string &title, u32 width, u32 height, WindowFlags flags) = 0;
        virtual void Poll() = 0;

        virtual int GetMonitorWidth() = 0;
        virtual int GetMonitorHeight() = 0;

        virtual void SetCursor(Cursor cursor) = 0;

    public:
        sig::signal<void(Key, ButtonState, KeyMod)> OnSetKeyState;
        sig::signal<void(KeyMod, MouseButton, ButtonState, const glm::ivec2 &)> OnSetMouseState;
        sig::signal<void(glm::ivec2, glm::ivec2)> OnSetMousePosition;
        sig::signal<void()> OnLoseFocus;
        sig::signal<void()> OnGainFocus;
        sig::signal<void(u32, u32)> OnResolutionChanged;

        sig::signal<void(u32, KeyMod)> OnChar;  // Text input

    public:
        virtual void *GetHandle() = 0;

        u32 GetWidth() const
        {
            return m_Width;
        }

        u32 GetHeight() const
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
        u32 m_Width = 0;
        u32 m_Height = 0;

        bool m_ShouldClose = false;
        bool m_IsFullscreen = false;
        bool m_SizeEnded = true;

        Cursor m_CurrentCursor = Cursor::Arrow;
        glm::ivec2 m_CursorPos{};
    };

}  // namespace Lorr