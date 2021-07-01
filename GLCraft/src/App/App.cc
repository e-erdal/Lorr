#include "App.hh"

App::App( AppDesc const &Description )
{
    if ( Init() )
    {
        m_pWindow = new Window( Description.sTitle, Description.uWidth, Description.uHeight );

        Run();
    }
}

App::~App()
{
    delete m_pWindow;
}

bool App::Init()
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