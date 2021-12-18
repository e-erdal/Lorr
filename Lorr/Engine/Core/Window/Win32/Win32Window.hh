//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/BaseWindow.hh"

namespace lr
{
    class Win32Window : public BaseWindow
    {
    public:
        ~Win32Window();

        void Init(const std::string &title, u32 monitor, u32 width, u32 height, WindowFlags flags) override;
        void Poll() override;

        void SetCursor(Cursor eCursor) override;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public:
        void *GetHandle() override;

    private:
        HWND m_Handle = 0;
        HINSTANCE m_Instance = 0;
    };
}  // namespace lr