//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <vector>

#include "Utils/BitFlags.hh"
#include "Utils/Logger.hh"
#include "Utils/Math.hh"

#include <glm/glm.hpp>

#include <Tracy.hpp>
#include <Windows.h>
#include <signals.h>

#ifndef NDEBUG
    #define ENGINE_DEBUG 1
#else
    #define ENGINE_DEBUG 0
#endif

#define PrintError( err )                                                                                                                                                \
    {                                                                                                                                                                    \
        char *pszError;                                                                                                                                                  \
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr,                                            \
                       MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPTSTR) &pszError, 0, NULL );                                                                       \
        Console::Fatal( err " HR: {}", pszError );                                                                                                                       \
    }

#define SAFE_DELETE( var )                                                                                                                                               \
    if ( var )                                                                                                                                                           \
    {                                                                                                                                                                    \
        delete var;                                                                                                                                                      \
        var = NULL;                                                                                                                                                      \
    }

#define SAFE_RELEASE( var )                                                                                                                                              \
    if ( var )                                                                                                                                                           \
    {                                                                                                                                                                    \
        var->Release();                                                                                                                                                  \
        var = nullptr;                                                                                                                                                   \
    }