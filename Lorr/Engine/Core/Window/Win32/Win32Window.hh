//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/IWindow.hh"

namespace Lorr
{
    class Win32Window : public IWindow
    {
    public:
        ~Win32Window();

        void Init(const std::string &title, u32 width, u32 height, WindowFlags flags) override;
        void Poll() override;

        int GetMonitorWidth() override;
        int GetMonitorHeight() override;

        void SetCursor(Cursor eCursor) override;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public:
        void *GetHandle() override;

    private:
        HWND m_Handle = 0;
        HINSTANCE m_Instance = 0;
    };
}  // namespace Lorr