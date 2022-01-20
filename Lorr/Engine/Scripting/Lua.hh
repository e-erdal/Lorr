//
// Created on Monday 3rd January 2022 by e-erdal
//

#pragma once

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <LuaBridge/LuaBridge.h>

namespace lr::Lua
{
    extern lua_State *L;

    void Init();
    void Execute(const eastl::string &script);

}  // namespace lr::Lua