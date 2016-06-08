//
//  LuaState.h
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

struct lua_State;

namespace lua
{
    struct LuaState
    {
        LuaState();
        LuaState(const char* file);
        
        ~LuaState();
        
        operator lua_State*();
                
    private:
        
        lua_State* L;
    };    
}
