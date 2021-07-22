//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <string>

#include "Core/Systems/Helpers/InputVars.hh"

#include <Core/Utils/BitFlags.hh>

#include <stdint.h>

namespace Lorr
{
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
    BitFlags( WindowFlags );

    class Window
    {
    public:
        ~Window();

        void Init( const std::string &sTitle, uint32_t uWidth, uint32_t uHeight, WindowFlags eFlags );
        void Poll();

        int GetMonitorWidth();
        int GetMonitorHeight();

        void SetCursor( Cursor eCursor );

    public:
        signals::signal<void( Key, ButtonState, KeyMod )> OnSetKeyState;
        signals::signal<void( KeyMod, MouseButton, ButtonState, const glm::ivec2 & )> OnSetMouseState;
        signals::signal<void( glm::ivec2 )> OnSetMousePosition;
        signals::signal<void()> OnLoseFocus;
        signals::signal<void()> OnGainFocus;
        signals::signal<void( uint32_t, uint32_t )> OnResolutionChanged;

        signals::signal<void( uint32_t, KeyMod )> OnChar;  // Text input

    public:
        HWND GetHandle() const
        {
            return m_Handle;
        }

        uint32_t GetWidth() const
        {
            return m_uWidth;
        }

        uint32_t GetHeight() const
        {
            return m_uHeight;
        }

        bool ShouldClose() const
        {
            return m_bShouldClose;
        }

        bool IsFullscreen() const
        {
            return m_bIsFullscreen;
        }

        glm::ivec2 GetCursorPos() const
        {
            return m_iv2CursorPos;
        }

    private:
        static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    private:
        HWND m_Handle = 0;
        HINSTANCE m_Instance = 0;

        uint32_t m_uWidth = 0;
        uint32_t m_uHeight = 0;

        bool m_bShouldClose = false;
        bool m_bIsFullscreen = false;
        bool m_bSizeEnded = true;

        Cursor m_eCurrentCursor = Cursor::Arrow;
        glm::ivec2 m_iv2CursorPos{};
    };
}  // namespace Lorr