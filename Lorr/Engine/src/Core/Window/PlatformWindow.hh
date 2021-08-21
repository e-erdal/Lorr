//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#if _WIN32
    #include "Win32/Win32Window.hh"
typedef Lorr::Win32Window PlatformWindow;
#endif