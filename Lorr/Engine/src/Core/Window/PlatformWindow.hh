//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include <bx/bx.h>

#if BX_PLATFORM_WINDOWS
    #include "Win32/Win32Window.hh"
typedef Lorr::Win32Window PlatformWindow;
#endif