//
// Created on Wednesday 29th December 2021 by e-erdal
//

#pragma once

#include <cstdio>
#include <cstdarg>

namespace lr
{
    inline eastl::string Format(const char *fmt, ...)
    {
        char buffer[512];

        va_list var;
        va_start(var, fmt);
        vsprintf(buffer, fmt, var);
        va_end(var);

        return eastl::string(buffer, strlen(buffer));
    }

}  // namespace lr