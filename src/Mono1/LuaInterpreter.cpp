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
	
	const int result = ExecuteFile("hello.lua");
	if(result != 0)
		std::cout << "Error: " << lua_tostring(mLuaState, -1) << std::endl;
}

LuaInterpreter::~LuaInterpreter()
{
	lua_close(mLuaState);
}

int LuaInterpreter::ExecuteFile(const std::string& file) const
{
	return luaL_dofile(mLuaState, file.c_str());
}

int LuaInterpreter::ExecuteString(const std::string& string) const
{
	return luaL_dostring(mLuaState, string.c_str());
}
