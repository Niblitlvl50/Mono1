//
//  LuaState.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LuaState.h"
#include <string>
#include <stdexcept>

extern "C"
{
    #include "Lua-5.1.4/lua.h"
    #include "Lua-5.1.4/lauxlib.h"
    #include "Lua-5.1.4/lualib.h"
}


using namespace lua;

LuaState::LuaState()
{
    L = lua_open();
    luaL_openlibs(L);
}

LuaState::LuaState(const char* file)
{
    L = lua_open();
    luaL_openlibs(L);

    const bool failed = luaL_dofile(L, file);
    if(failed)
        throw std::runtime_error("Unable to load file: " + std::string(file));
}

LuaState::~LuaState()
{
    lua_close(L);
}

LuaState::operator lua_State*()
{
    return L;
}



