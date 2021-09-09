//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include "Common.hh"

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/bx.h>
#include <glm/glm.hpp>
#include <signals.h>
#include <Tracy.hpp>

#include "Engine/Utils/BitFlags.hh"
#include "Engine/Utils/Logger.hh"
#include "Engine/Utils/Math.hh"
#include "Engine/Utils/FPSLimiter.hh"
#include "Engine/Utils/Timer.hh"
#include "Engine/Utils/Identifier.hh"

#include <Windows.h>
#include <stdint.h>
#include <vector>

#ifndef NDEBUG
    #define ENGINE_DEBUG 1
#else
    #define ENGINE_DEBUG 0
#endif

#define PrintError(err)                                                                                                                                                  \
    {                                                                                                                                                                    \
        char *pszError;                                                                                                                                                  \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  \
                      (LPTSTR)&pszError, 0, NULL);                                                                                                                       \
        Console::Fatal(err " HR: {}", pszError);                                                                                                                         \
    }

#define SAFE_DELETE(var)                                                                                                                                                 \
    if (var)                                                                                                                                                             \
    {                                                                                                                                                                    \
        delete var;                                                                                                                                                      \
        var = NULL;                                                                                                                                                      \
    }

#define SAFE_RELEASE(var)                                                                                                                                                \
    if (var)                                                                                                                                                             \
    {                                                                                                                                                                    \
        var->Release();                                                                                                                                                  \
        var = nullptr;                                                                                                                                                   \
    }

#define _REALLOC(x, len) (uint8_t *)realloc((void *)x, len)
#define _MALLOC(len) (uint8_t *)malloc(len)
#define _ZEROM(x, len) memset((void *)x, 0, len)

#define PACK_VERSION(major, minor, build) ((uint32_t)((uint8_t)major << 24 | (uint8_t)minor << 16 | _byteswap_ushort((uint16_t)build)))
#define UNPACK_VERSION(packedVersion, major, minor, build)                                                                                                               \
    {                                                                                                                                                                    \
        major = (uint8_t)((uint32_t)packedVersion >> 24);                                                                                                                \
        minor = (uint8_t)((uint32_t)packedVersion >> 16);                                                                                                                \
        build = (uint16_t)_byteswap_ushort((uint16_t)packedVersion);                                                                                                     \
    }