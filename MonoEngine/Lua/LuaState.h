//
//  LuaState.h
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <string>

struct lua_State;

namespace lua
{
    struct LuaState
    {
        LuaState();
        LuaState(const std::string& file);
        
        ~LuaState();
        
        operator lua_State*();
                
    private:
        
        lua_State* L;
    };    
}
