/*
 *  LuaInterpreter.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MONO_LUAINTERPRETER_H
#define MONO_LUAINTERPRETER_H

#include <string>

class lua_State;

namespace mono
{
    
	class LuaInterpreter
	{
	public:
		LuaInterpreter();
		~LuaInterpreter();
		
		int ExecuteFile(const std::string& file) const;
		int ExecuteString(const std::string& string) const;
		
	private:
	
		lua_State* mLuaState;
	};
    
}


#endif

