#include "Lua.hh"

namespace lr
{
    lua_State *Lua::L = nullptr;

    void Lua::Init()
    {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    void Lua::Execute(const eastl::string &script)
    {
        i32 status;
        if ((status = luaL_dostring(L, script.c_str())) != 0)
        {
            LOG_ERROR("Failed to execute script! Error: %s\n", lua_tostring(L, -1));
        }
    }

}  // namespace lr