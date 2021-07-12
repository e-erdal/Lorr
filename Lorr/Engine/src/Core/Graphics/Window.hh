//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <string>

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
        HWND GetHandle() const
        {
            return m_Handle;
        }

        uint32_t GetWidth() const
        {
            return uWidth;
        }

        uint32_t GetHeight() const
        {
            return uHeight;
        }

        bool ShouldClose() const
        {
            return m_bShouldClose;
        }

        bool IsFullscreen() const
        {
            return m_bIsFullscreen;
        }

    private:
        static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    private:
        HWND m_Handle = 0;
        HINSTANCE m_Instance = 0;

        uint32_t uWidth = 0;
        uint32_t uHeight = 0;

        bool m_bShouldClose = false;
        bool m_bIsFullscreen = false;

        Cursor m_eCurrentCursor = Cursor::Arrow;
    };
}  // namespace Lorr