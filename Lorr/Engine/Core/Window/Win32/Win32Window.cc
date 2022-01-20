#include "Win32Window.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    constexpr DWORD g_defWindowStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_POPUP | WS_VISIBLE);

    Key TranslateWin32KeySym(WPARAM wParam)
    {
        // 12345... + QWERTY...
        if (wParam >= 0x30 && wParam <= 0x5A) return (Key)wParam;
        // NUMPAD0-9 + /*-+
        if (wParam >= 0x60 && wParam <= 0x69) return (Key)(wParam + 224);

        switch (wParam)
        {
            case VK_ESCAPE: return Key::Key_ESCAPE;
            case VK_TAB: return Key::Key_TAB;
            case VK_LSHIFT: return Key::Key_LEFT_SHIFT;
            case VK_RSHIFT: return Key::Key_RIGHT_SHIFT;
            case VK_LCONTROL: return Key::Key_LEFT_CONTROL;
            case VK_RCONTROL: return Key::Key_RIGHT_CONTROL;
            case VK_LMENU: return Key::Key_LEFT_ALT;
            case VK_RMENU: return Key::Key_RIGHT_ALT;
            case VK_NUMLOCK: return Key::Key_NUM_LOCK;
            case VK_CAPITAL: return Key::Key_CAPS_LOCK;
            case VK_SNAPSHOT: return Key::Key_PRINT_SCREEN;
            case VK_SCROLL: return Key::Key_SCROLL_LOCK;
            case VK_PAUSE: return Key::Key_PAUSE;
            case VK_DELETE: return Key::Key_DELETE;
            case VK_BACK: return Key::Key_BACKSPACE;
            case VK_RETURN: return Key::Key_ENTER;
            case VK_HOME: return Key::Key_HOME;
            case VK_END: return Key::Key_END;
            case VK_PRIOR: return Key::Key_PAGE_UP;
            case VK_NEXT: return Key::Key_PAGE_DOWN;
            case VK_LEFT: return Key::Key_LEFT;
            case VK_RIGHT: return Key::Key_RIGHT;
            case VK_DOWN: return Key::Key_DOWN;
            case VK_UP: return Key::Key_UP;
            case VK_SPACE: return Key::Key_SPACE;
            case VK_LWIN: return Key::Key_LEFT_SUPER;
            case VK_RWIN: return Key::Key_RIGHT_SUPER;
            case VK_APPS: return Key::Key_MENU;
            case VK_INSERT: return Key::Key_INSERT;

            case VK_ADD: return Key::Key_KP_ADD;
            case VK_DECIMAL:
            case VK_SEPARATOR: return Key::Key_KP_DECIMAL;
            case VK_DIVIDE: return Key::Key_KP_DIVIDE;
            case VK_MULTIPLY: return Key::Key_KP_MULTIPLY;
            case VK_SUBTRACT: return Key::Key_KP_SUBTRACT;

            case VK_F1: return Key::Key_F1;
            case VK_F2: return Key::Key_F2;
            case VK_F3: return Key::Key_F3;
            case VK_F4: return Key::Key_F4;
            case VK_F5: return Key::Key_F5;
            case VK_F6: return Key::Key_F6;
            case VK_F7: return Key::Key_F7;
            case VK_F8: return Key::Key_F8;
            case VK_F9: return Key::Key_F9;
            case VK_F10: return Key::Key_F10;
            case VK_F11: return Key::Key_F11;
            case VK_F12: return Key::Key_F12;
            case VK_F13: return Key::Key_F13;
            case VK_F14: return Key::Key_F14;
            case VK_F15: return Key::Key_F15;
            case VK_F16: return Key::Key_F16;
            case VK_F17: return Key::Key_F17;
            case VK_F18: return Key::Key_F18;
            case VK_F19: return Key::Key_F19;
            case VK_F20: return Key::Key_F20;
            case VK_F21: return Key::Key_F21;
            case VK_F22: return Key::Key_F22;
            case VK_F23: return Key::Key_F23;
            case VK_F24: return Key::Key_F24;
        }

        return Key::Key_UNKNOWN;
    }

    Win32Window::~Win32Window()
    {
    }

    void Win32Window::Init(const eastl::string &title, u32 monitor, u32 width, u32 height, WindowFlags flags)
    {
        ZoneScoped;

        m_Flags = flags;
        m_UsingMonitor = monitor;

        //* Initialize System Metrics *//
        /// Get info of display[n]
        for (u32 currentDevice = 0; currentDevice < GetSystemMetrics(SM_CMONITORS); currentDevice++)
        {
            DEVMODE devMode = {};
            DISPLAY_DEVICE displayDevice = {};
            DISPLAY_DEVICE monitorName = {};
            displayDevice.cb = sizeof(DISPLAY_DEVICE);
            monitorName.cb = sizeof(DISPLAY_DEVICE);
            EnumDisplayDevices(NULL, currentDevice, &displayDevice, 0);
            EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
            EnumDisplayDevices(displayDevice.DeviceName, 0, &monitorName, 0);

            SystemMetrics::Display display;
            display.Name = monitorName.DeviceString;
            display.RefreshRate = devMode.dmDisplayFrequency;
            display.ResW = devMode.dmPelsWidth;
            display.ResH = devMode.dmPelsHeight;
            display.PosX = devMode.dmPosition.x;
            display.PosY = devMode.dmPosition.y;

            m_SystemMetrics.Displays.push_back(display);
        }

        auto currentDisplay = GetDisplay(monitor);
        if (!currentDisplay)
        {
            LOG_ERROR("DISPLAY%d is not available?", monitor + 1);
            return;
        }

        if (width == 0 && height == 0)
        {
            width = currentDisplay->ResW;
            height = currentDisplay->ResH;
        }

        LOG_TRACE("Creating new window \"%s\"<%d, %d>", title.c_str(), width, height);

        // Getting ready for window
        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wc.lpfnWndProc = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_Instance;
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = 0;
        wc.lpszClassName = "Lorr";
        wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

        // Create window
        RegisterClassEx(&wc);

        int windowFlags = g_defWindowStyle;
        if (flags & WindowFlags::Fullscreen)
        {
            LOG_TRACE("Getting ready for fullscreen state.");

            // actual holy shit moment
            if ((width != currentDisplay->ResW) && (height != currentDisplay->ResH))
            {
                // window will be fullscreen, no need to calculate other position and stuff
                // just directly pass it over
                DEVMODE dm = {};

                dm.dmSize = sizeof(DEVMODE);
                dm.dmPelsWidth = width;
                dm.dmPelsHeight = height;
                dm.dmBitsPerPel = 32;
                dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

                if (ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
                {
                    // throw error
                    LOG_ERROR("Fullscreen is not supported by the GPU.");
                }
            }

            m_Handle = CreateWindowEx(0, wc.lpszClassName, title.c_str(), WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, currentDisplay->ResW,
                                      currentDisplay->ResH, 0, 0, 0, 0);

            m_IsFullscreen = true;
        }

        else
        {
            if (flags & WindowFlags::Resizable) windowFlags |= WS_MAXIMIZEBOX | WS_THICKFRAME;

            int windowPosX = currentDisplay->PosX;
            int windowPosY = currentDisplay->PosY;

            if (flags & WindowFlags::Centered)
            {
                windowPosX += (currentDisplay->ResW / 2) - (width / 2);
                windowPosY += (currentDisplay->ResH / 2) - (height / 2);
            }

            RECT rc = { 0, 0, (long)width, (long)height };

            AdjustWindowRectEx(&rc, windowFlags, 0, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

            m_Handle =
                CreateWindowEx(0, wc.lpszClassName, title.c_str(), windowFlags, windowPosX, windowPosY, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0);
        }

        ShowWindow(m_Handle, SW_SHOW);
        UpdateWindow(m_Handle);

        LOG_TRACE("Successfully created window.");

        m_Width = width;
        m_Height = height;
    }

    void Win32Window::Poll()
    {
        ZoneScoped;

        MSG msg;

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Win32Window::SetCursor(Cursor cursor)
    {
        ZoneScoped;

        ::ShowCursor(TRUE);  // make sure that cursor is shown

        static ::HCURSOR arrowCursor{ LoadCursor(NULL, IDC_ARROW) };
        static ::HCURSOR ibeamCursor{ LoadCursor(NULL, IDC_IBEAM) };
        static ::HCURSOR handCursor{ LoadCursor(NULL, IDC_HAND) };
        static ::HCURSOR sizeAllCursor{ LoadCursor(NULL, IDC_SIZEALL) };
        static ::HCURSOR sizeWECursor{ LoadCursor(NULL, IDC_SIZEWE) };
        static ::HCURSOR sizeNSCursor{ LoadCursor(NULL, IDC_SIZENS) };
        static ::HCURSOR sizeNESWCursor{ LoadCursor(NULL, IDC_SIZENESW) };
        static ::HCURSOR sizeNWSECursor{ LoadCursor(NULL, IDC_SIZENWSE) };
        static ::HCURSOR noCursor{ LoadCursor(NULL, IDC_NO) };

        switch (cursor)
        {
            case Cursor::Arrow: ::SetCursor(arrowCursor); break;
            case Cursor::TextInput: ::SetCursor(ibeamCursor); break;
            case Cursor::ResizeAll: ::SetCursor(sizeAllCursor); break;
            case Cursor::ResizeEW: ::SetCursor(sizeWECursor); break;
            case Cursor::ResizeNS: ::SetCursor(sizeNSCursor); break;
            case Cursor::ResizeNESW: ::SetCursor(sizeNESWCursor); break;  // FIXME:
            case Cursor::ResizeNWSE: ::SetCursor(sizeNWSECursor); break;
            case Cursor::Hand: ::SetCursor(handCursor); break;
            case Cursor::NotAllowed: ::SetCursor(noCursor); break;
            case Cursor::Hidden: ::ShowCursor(FALSE); break;
        }

        m_CurrentCursor = cursor;
    }

    LRESULT CALLBACK Win32Window::WindowProc(HWND hHwnd, UINT uMSG, WPARAM wParam, LPARAM lParam)
    {
        ZoneScoped;

        Engine *pEngine = GetEngine();

        if (!pEngine) return DefWindowProc(hHwnd, uMSG, wParam, lParam);

        Win32Window *pWindow = (Win32Window *)pEngine->GetWindow();
        InputManager *pInputMan = pEngine->GetInputMan();

        switch (uMSG)
        {
            case WM_DESTROY:
            case WM_CLOSE: pWindow->m_ShouldClose = true; break;
            case WM_SIZE:
            {
                if (pWindow->m_SizeEnded)
                {
                    LOG_TRACE("Window size changed to %d, %d", LOWORD(lParam), HIWORD(lParam));
                    pWindow->OnResolutionChanged((u32)LOWORD(lParam), (u32)HIWORD(lParam));
                    pWindow->m_Width = (u32)LOWORD(lParam);
                    pWindow->m_Height = (u32)HIWORD(lParam);
                }

                break;
            }
            case WM_EXITSIZEMOVE:
                pWindow->m_SizeEnded = true;
                RECT rc;
                GetClientRect(pWindow->m_Handle, &rc);
                LOG_TRACE("Window size changed to %ld, %ld", rc.right, rc.bottom);
                pWindow->OnResolutionChanged((u32)rc.right, (u32)rc.bottom);
                pWindow->m_Width = (u32)rc.right;
                pWindow->m_Height = (u32)rc.bottom;

                break;
            case WM_ENTERSIZEMOVE: pWindow->m_SizeEnded = false; break;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONDBLCLK:
            {
                MouseButton keys = MouseButton::NONE;
                if (wParam & MK_LBUTTON) keys |= MouseButton::BTN_1;
                if (wParam & MK_RBUTTON) keys |= MouseButton::BTN_2;

                KeyMod mods = KeyMod::None;

                if (GetKeyState(VK_SHIFT) & 0x8000) mods |= KeyMod::SHIFT;
                if (GetKeyState(VK_CONTROL) & 0x8000) mods |= KeyMod::CONTROL;
                if (GetKeyState(VK_MENU) & 0x8000) mods |= KeyMod::ALT;
                if (GetKeyState(VK_LWIN) & 0x8000) mods |= KeyMod::SUPER;
                if (GetKeyState(VK_CAPITAL) & 0x8000) mods |= KeyMod::CAPS_LOCK;

                ButtonState state;

                if (uMSG == WM_LBUTTONDOWN || uMSG == WM_RBUTTONDOWN)
                    state = ButtonState::Pressed;
                else
                    state = ButtonState::DoubleClicked;

                pInputMan->OnMouseState(state, keys, mods);
                pInputMan->OnMousePosUpdate(glm::ivec2(LOWORD(lParam), HIWORD(lParam)), {});

                break;
            }
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            {
                MouseButton keys = MouseButton::NONE;
                if (uMSG == WM_LBUTTONUP) keys = MouseButton::BTN_1;
                if (uMSG == WM_RBUTTONUP) keys = MouseButton::BTN_2;
                if (uMSG == WM_MBUTTONUP) keys = MouseButton::BTN_3;

                KeyMod mods = KeyMod::None;

                if (GetKeyState(VK_SHIFT) & 0x8000) mods |= KeyMod::SHIFT;
                if (GetKeyState(VK_CONTROL) & 0x8000) mods |= KeyMod::CONTROL;
                if (GetKeyState(VK_MENU) & 0x8000) mods |= KeyMod::ALT;
                if (GetKeyState(VK_LWIN) & 0x8000) mods |= KeyMod::SUPER;
                if (GetKeyState(VK_CAPITAL) & 0x8000) mods |= KeyMod::CAPS_LOCK;

                pInputMan->OnMouseState(ButtonState::Released, keys, mods);
                pInputMan->OnMousePosUpdate(glm::ivec2(LOWORD(lParam), HIWORD(lParam)), {});

                break;
            }

            case WM_MOUSEMOVE:
            {
                static bool first = true;
                static int lastX, lastY;
                int offsetX = 0, offsetY = 0;

                int x = ((int)(short)LOWORD(lParam));
                int y = ((int)(short)HIWORD(lParam));

                if (first)
                {
                    lastX = x;
                    lastY = y;

                    first = false;
                }
                else
                {
                    offsetX = x - lastX;
                    offsetY = y - lastY;

                    lastX = x;
                    lastY = y;
                }

                pInputMan->OnMousePosUpdate(glm::ivec2(x, y), glm::ivec2(offsetX, offsetY));

                break;
            }
            case WM_KEYUP:
            case WM_KEYDOWN:
            {
                Key key = TranslateWin32KeySym(wParam);

                KeyMod mods = KeyMod::None;

                if (GetKeyState(VK_SHIFT) & 0x8000) mods |= KeyMod::SHIFT;
                if (GetKeyState(VK_CONTROL) & 0x8000) mods |= KeyMod::CONTROL;
                if (GetKeyState(VK_MENU) & 0x8000) mods |= KeyMod::ALT;
                if (GetKeyState(VK_LWIN) & 0x8000) mods |= KeyMod::SUPER;
                if (GetKeyState(VK_CAPITAL) & 0x8000) mods |= KeyMod::CAPS_LOCK;

                pInputMan->OnKeyboardState(uMSG == WM_KEYDOWN ? ButtonState::Pressed : ButtonState::Released, key, mods);

                break;
            }
            case WM_SETCURSOR:
            {
                if (LOWORD(lParam) == 1)
                {
                    pWindow->SetCursor(pWindow->m_CurrentCursor);
                    return true;
                }

                return DefWindowProc(hHwnd, uMSG, wParam, lParam);
            }

            default: return DefWindowProc(hHwnd, uMSG, wParam, lParam);
        }

        return 0;
    }

    void *Win32Window::GetHandle()
    {
        return m_Handle;
    }
}  // namespace lr