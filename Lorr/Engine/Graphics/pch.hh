//
// Created on Thursday 2nd December 2021 by e-erdal
// Special PCH for Graphics module of Lorr.
//

#pragma once

#if LR_BACKEND_D3D12
#include <directx/d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#endif

#if LR_BACKEND_D3D11
#include <d3d11.h>
#endif

#define HRCall(func, message)                                                                                                                        \
    if (FAILED(hr = func))                                                                                                                           \
    {                                                                                                                                                \
        char *pError;                                                                                                                                \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr,                         \
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pError, 0, NULL);                                                          \
        LOG_ERROR(message "HR: %s", pError);                                                                                                         \
        return;                                                                                                                                      \
    }

#define HRCallRet(func, message, ret)                                                                                                                \
    if (FAILED(hr = func))                                                                                                                           \
    {                                                                                                                                                \
        char *pError;                                                                                                                                \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr,                         \
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pError, 0, NULL);                                                          \
        LOG_ERROR(message "HR: %s", pError);                                                                                                         \
        return ret;                                                                                                                                  \
    }