#include "App.hh"

App::App( AppDesc const &Description )
{
    if ( PreInit() )
    {
        m_pWindow = new Window( Description.sTitle, Description.uWidth, Description.uHeight );
        m_pCamera = new Camera( { 0, 0, 0 }, { 0, 1, 0 }, { Description.uWidth, Description.uHeight }, 60.f, 0.0001f, 10000.f );
        m_pVertexBatcher = new VertexBatcher();

        m_MainShader.Load( "res/shaders/main" );

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
        BeginFrame();

        EndFrame();

        m_pWindow->Poll();
    }
}

void App::BeginFrame()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_MainShader.Bind();
    m_MainShader.SetMat4( "uTransform", m_pCamera->GetTransform() );

    m_pVertexBatcher->BeginFrame();
}

void App::EndFrame()
{
    m_pVertexBatcher->EndFrame();

    m_MainShader.Unbind();
}