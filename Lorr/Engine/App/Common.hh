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

#define SAFE_FREE(var)                                                                                                                                                   \
    if (var)                                                                                                                                                             \
    {                                                                                                                                                                    \
        free(var);                                                                                                                                                       \
        var = NULL;                                                                                                                                                      \
    }

#define SAFE_RELEASE(var)                                                                                                                                                \
    if (var != nullptr)                                                                                                                                                             \
    {                                                                                                                                                                    \
        var->Release();                                                                                                                                                  \
        var = nullptr;                                                                                                                                                   \
    }

#define _REALLOC(x, len) (u8 *)realloc((void *)x, len)
#define _MALLOC(len) (u8 *)malloc(len)
#define _ZEROM(x, len) memset((void *)x, 0, len)

#define PACK_VERSION(major, minor, build) ((u32)((u8)major << 28 | ((u16)minor & 0x0fff) << 16 | _byteswap_ushort((u16)build)))
#define UNPACK_VERSION(packedVersion, major, minor, build)                                                                                                               \
    {                                                                                                                                                                    \
        major = ((u8)((u32)packedVersion >> 24) >> 4);                                                                                                         \
        minor = (u16)(((u32)packedVersion >> 16) & 0x0fff);                                                                                                    \
        build = _byteswap_ushort((u16)packedVersion & 0x0000ffff);                                                                                                  \
    }

//
// Framework defines
//

#define ENGINE_VERSION_MAJOR 1
#define ENGINE_VERSION_MINOR 2

#ifndef ENGINE_VERSION_PATCH
    #define ENGINE_VERSION_PATCH 0
#endif

#ifndef ENGINE_VERSION_GITHASH
    #define ENGINE_VERSION_GITHASH "ffffffff"
#endif

#define ENGINE_VERSION_TAG "alpha"
#define ENGINE_VERSION_PACKED PACK_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)

typedef unsigned long long u64;
typedef signed long long i64;

typedef unsigned int u32;
typedef signed int i32;

typedef unsigned short u16;
typedef signed short i16;

typedef unsigned char u8;
typedef signed char i8;