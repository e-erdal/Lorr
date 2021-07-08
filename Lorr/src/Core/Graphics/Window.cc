#include "Window.hh"

#include "Core/Engine/Engine.hh"
#include "Core/Utils/Logger.hh"

constexpr DWORD g_defWindowStyle = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_POPUP | WS_VISIBLE );

Window::Window( const std::string &sTitle, uint32_t uWidth, uint32_t uHeight, WindowFlags eFlags )
{
    Console::Log( "Creating new window \"{}\"<{}, {}>", sTitle, uWidth, uHeight );

    // Getting ready for window
    WNDCLASSEX wc;
    ZeroMemory( &wc, sizeof( WNDCLASSEX ) );
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_Instance;
    wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = 0;
    wc.lpszClassName = "Lorr";
    wc.hIconSm = LoadIcon( 0, IDI_APPLICATION );

    // Create window
    RegisterClassEx( &wc );

    int windowFlags = g_defWindowStyle;
    if ( eFlags & WindowFlags::Fullscreen )
    {
        Console::Log( "Getting ready for fullscreen state." );

        // window will be fullscreen, no need to calculate other position and stuff
        // just directly pass it over
        DEVMODE dm;
        ZeroMemory( &dm, sizeof( DEVMODE ) );

        dm.dmSize = sizeof( DEVMODE );
        dm.dmPelsWidth = GetMonitorWidth();
        dm.dmPelsHeight = GetMonitorHeight();
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // actual holy shit moment
        if ( ( uWidth != GetMonitorWidth() ) && ( uHeight != GetMonitorHeight() ) )
        {
            if ( ChangeDisplaySettingsA( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
            {
                // throw error
                Console::Error( "Fullscreen is not supported by the GPU." );
            }
        }

        m_Handle = CreateWindowEx( 0, wc.lpszClassName, sTitle.c_str(), WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, GetMonitorWidth(),
                                   GetMonitorHeight(), 0, 0, 0, 0 );

        m_bIsFullscreen = true;
    }

    else
    {
        if ( eFlags & WindowFlags::Resizable ) windowFlags |= WS_MAXIMIZEBOX | WS_THICKFRAME;

        int windowPosX = 0;
        int windowPosY = 0;

        if ( eFlags & WindowFlags::Centered )
        {
            windowPosX = ( GetMonitorWidth() / 2 ) - ( uWidth / 2 );
            windowPosY = ( GetMonitorHeight() / 2 ) - ( uHeight / 2 );
        }

        RECT rc = { 0, 0, (long) uWidth, (long) uHeight };

        AdjustWindowRectEx( &rc, windowFlags, 0, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );

        m_Handle = CreateWindowEx( 0, wc.lpszClassName, sTitle.c_str(), windowFlags, windowPosX, windowPosY, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0 );
    }

    ShowWindow( m_Handle, SW_SHOW );
    UpdateWindow( m_Handle );

    Console::Log( "Successfully created window." );
}

Window::~Window()
{
}

void Window::Poll()
{
    MSG msg;

    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
}

int Window::GetMonitorWidth()
{
    return GetSystemMetrics( SM_CXSCREEN );
}

int Window::GetMonitorHeight()
{
    return GetSystemMetrics( SM_CYSCREEN );
}

LRESULT CALLBACK Window::WindowProc( HWND hHwnd, UINT uMSG, WPARAM wParam, LPARAM lParam )
{
    Engine *pEngine = GetEngine();

    if ( !pEngine ) return DefWindowProc( hHwnd, uMSG, wParam, lParam );

    Window *pWindow = pEngine->GetWindow();

    switch ( uMSG )
    {
    case WM_DESTROY:
    case WM_CLOSE: pWindow->m_bShouldClose = true; break;

    default: return DefWindowProc( hHwnd, uMSG, wParam, lParam );
    }

    return 0;
}