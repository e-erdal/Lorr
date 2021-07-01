#include "App.hh"

App::App( AppDesc const &Description )
{
    if ( PreInit() )
    {
        m_pWindow = new Window( Description.sTitle, Description.uWidth, Description.uHeight );
        m_pCamera = new Camera( { 0, 0, 0 }, { 0, 1, 0 }, { Description.uWidth, Description.uHeight }, 60.f, 0.0001f, 10000.f );

        Run();
    }
}

App::~App()
{
    delete m_pWindow;
}

bool App::PreInit()
{
    if ( glfwInit() != GLFW_TRUE )
    {
        printf( "Failed to initialize GLFW.\n" );
        return false;
    }

    return true;
}

void App::Run()
{
    while ( !m_pWindow->ShouldClose() )
    {
        m_pWindow->Poll();
    }
}