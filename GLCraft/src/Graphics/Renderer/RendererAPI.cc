#include "RendererAPI.hh"

void RendererAPI::SetViewport( int iWidth, int iHeight )
{
    glViewport( 0, 0, iWidth, iHeight );
}