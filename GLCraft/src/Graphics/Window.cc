#include "Window.hh"

static void OnScreenResChanged( GLFWwindow *window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

Window::Window( const std::string &sTitle, uint32_t uWidth, uint32_t uHeight )
{
    // Getting ready for window
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );

    // Create window
    m_pHandle = glfwCreateWindow( uWidth, uHeight, sTitle.c_str(), 0, 0 );
    if ( !m_pHandle )
    {
        printf( "Failed to create window.\n" );
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent( m_pHandle );

    // Create OGL
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) )
    {
        printf( "Failed to create OpenGL context.\n" );
        glfwTerminate();
        return;
    }

    // Initialize callbacks
    glfwSetFramebufferSizeCallback( m_pHandle, OnScreenResChanged );

    // Initialize OpenGL options
    glViewport( 0, 0, uWidth, uHeight );
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Poll()
{
    glfwSwapBuffers( m_pHandle );
    glfwPollEvents();
}