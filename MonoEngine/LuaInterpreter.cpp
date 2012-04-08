/*
 *  LuaInterpreter.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "LuaInterpreter.h"
#include <iostream>
#include <stdexcept>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

using namespace mono;

LuaInterpreter::LuaInterpreter()
{
	mLuaState = lua_open();
	luaL_openlibs(mLuaState);
}

LuaInterpreter::~LuaInterpreter()
{
	lua_close(mLuaState);
}

void LuaInterpreter::RegisterFunction(const std::string& name, const LuaFunction& func)
{
    typedef int (*function_t)( lua_State* );
    const function_t* function = func.target<const function_t>();
    if(!function)
        throw std::runtime_error("Not a stand alone function :(");
    
    lua_register(mLuaState, name.c_str(), *function);
}

int LuaInterpreter::ExecuteFile(const std::string& file) const
{
	return luaL_dofile(mLuaState, file.c_str());
}

int LuaInterpreter::ExecuteString(const std::string& string) const
{
	return luaL_dostring(mLuaState, string.c_str());
}
