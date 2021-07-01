//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <string>

class Shader
{
public:
    void Load( const std::string &sPath );
    void Bind();
    void Unbind();

    void SetInt( const char *szUniform, int iVal );
    void SetFloat( const char *szUniform, float fVal );
    void SetMat4( const char *szUniform, const glm::mat4 &m4Val );

    operator GLuint()
    {
        return m_ID;
    }

    operator GLint()
    {
        return (GLint) m_ID;
    }

private:
    void Compile( GLuint uShaderID, const char *szData );
    int Uniform( const char *szUniform );

    GLuint m_ID;
};