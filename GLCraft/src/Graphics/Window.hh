//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include <stdint.h>

class Window
{
public:
    Window( const std::string &sTitle, uint32_t uWidth, uint32_t uHeight );
    ~Window();

    void Poll();

public:
    GLFWwindow *GetHandle() const
    {
        return m_pHandle;
    }

    uint32_t GetWidth() const
    {
        return uWidth;
    }

    uint32_t GetHeight() const
    {
        return uHeight;
    }

    bool ShouldClose() const
    {
        return glfwWindowShouldClose( m_pHandle );
    }

private:
    GLFWwindow *m_pHandle = 0;

    uint32_t uWidth = 0;
    uint32_t uHeight = 0;
};