//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

namespace lr
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

    struct SystemMetrics
    {
        struct Display
        {
            eastl::string Name;

            u32 ResW;
            u32 ResH;
            u32 PosX;
            u32 PosY;

            u32 RefreshRate;
        };

        eastl::vector<Display> Displays;
    };

    class BaseWindow
    {
    public:
        virtual ~BaseWindow(){};

        virtual void Init(const eastl::string &title, u32 monitor, u32 width, u32 height, WindowFlags flags) = 0;
        virtual void Poll() = 0;

        const SystemMetrics::Display *GetDisplay(u32 monitor) const;

        virtual void SetCursor(Cursor cursor) = 0;

    public:
        sig::signal<void()> OnLoseFocus;
        sig::signal<void()> OnGainFocus;
        sig::signal<void(u32, u32)> OnResolutionChanged;

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

        /// This should not be confused with InputManager::GetMousePos
        /// This function gets coordinates of monitor space mouse
        /// Meanwhile InputManager gets window space mouse coordinates
        glm::ivec2 GetCursorPos()
        {
            ZoneScoped;

            LPPOINT point;
            ::GetCursorPos(point);

            return glm::ivec2(point->x, point->y);
        }

        u32 GetUsingMonitor()
        {
            return m_UsingMonitor;
        }

    protected:
        u32 m_Width = 0;
        u32 m_Height = 0;

        bool m_ShouldClose = false;
        bool m_IsFullscreen = false;
        bool m_SizeEnded = true;

        Cursor m_CurrentCursor = Cursor::Arrow;

        WindowFlags m_Flags;
        SystemMetrics m_SystemMetrics;
        u32 m_UsingMonitor = 0;
    };

}  // namespace lr