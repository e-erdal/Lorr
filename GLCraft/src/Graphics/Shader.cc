#include <fstream>
#include <sstream>
#include <vector>

#include "Shader.hh"

void Shader::Load( const std::string &sPath )
{
    int VID = glCreateShader( GL_VERTEX_SHADER );
    int FID = glCreateShader( GL_FRAGMENT_SHADER );

    std::string vcode;
    std::string fcode;

    std::ifstream vstream( sPath + ".v.glsl", std::ios::in );
    std::ifstream fstream( sPath + ".f.glsl", std::ios::in );

    if ( !vstream.is_open() )
    {
        printf( "Unable to open vertex shader.\n" );
        return;
    }

    if ( !fstream.is_open() )
    {
        printf( "Unable to open fragment shader.\n" );
        return;
    }

    std::stringstream ss;

    ss << vstream.rdbuf();
    vcode = ss.str();
    vstream.close();

    ss.str( "" );

    ss << fstream.rdbuf();
    fcode = ss.str();
    fstream.close();

    printf( "Compiling '%s.v.glsl'\n", sPath.c_str() );
    Compile( VID, vcode.c_str() );

    printf( "Compiling '%s.f.glsl'\n", sPath.c_str() );
    Compile( FID, fcode.c_str() );

    m_ID = glCreateProgram();

    glAttachShader( m_ID, VID );
    glAttachShader( m_ID, FID );
    glLinkProgram( m_ID );

    glDetachShader( m_ID, VID );
    glDetachShader( m_ID, FID );

    glDeleteShader( VID );
    glDeleteShader( FID );
}

void Shader::Bind()
{
    glUseProgram( m_ID );
}

void Shader::Unbind()
{
    glUseProgram( 0 );
}

void Shader::Compile( GLuint uShaderID, const char *szData )
{
    glShaderSource( uShaderID, 1, &szData, NULL );
    glCompileShader( uShaderID );

    GLint result;
    int msgLen;

    glGetShaderiv( uShaderID, GL_COMPILE_STATUS, &result );
    glGetShaderiv( uShaderID, GL_INFO_LOG_LENGTH, &msgLen );

    if ( msgLen > 0 )
    {
        std::vector<char> msg( msgLen + 1 );
        glGetShaderInfoLog( uShaderID, msgLen, NULL, &msg[0] );
        printf( "%s\n", &msg[0] );
    }

    printf("Compiled shader.\n");
}

int Shader::Uniform( const char *szUniform )
{
    return glGetUniformLocation( m_ID, szUniform );
}

void Shader::SetInt( const char *szUniform, int iVal )
{
    glUniform1i( Uniform( szUniform ), iVal );
}

void Shader::SetFloat( const char *szUniform, float fVal )
{
    glUniform1f( Uniform( szUniform ), fVal );
}

void Shader::SetMat4( const char *szUniform, const glm::mat4 &m4Val )
{
    glUniformMatrix4fv( Uniform( szUniform ), 1, GL_FALSE, &m4Val[0][0] );
}