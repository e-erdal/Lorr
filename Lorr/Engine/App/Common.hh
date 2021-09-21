//
// Created on August 26th 2021 by e-erdal.
//

#pragma once

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

#define PACK_VERSION(major, minor, build) ((uint32_t)((uint8_t)major << 28 | ((uint16_t)minor & 0x0fff) << 16 | _byteswap_ushort((uint16_t)build)))
#define UNPACK_VERSION(packedVersion, major, minor, build)                                                                                                               \
    {                                                                                                                                                                    \
        major = ((uint8_t)((uint32_t)packedVersion >> 24) >> 4);                                                                                                         \
        minor = (uint16_t)(((uint32_t)packedVersion >> 16) & 0x0fff);                                                                                                    \
        build = _byteswap_ushort((uint16_t)packedVersion & 0x0000ffff);                                                                                                  \
    }

//
// Framework defines
//

#define ENGINE_VERSION_MAJOR 1
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_TAG "alpha"
#define ENGINE_VERSION_PACKED

#ifndef ENGINE_VERSION_GITHASH
#define ENGINE_VERSION_GITHASH 0xffffffff
#endif
