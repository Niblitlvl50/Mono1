//
//  LuaHelpers.h
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>


namespace lua
{
    struct LuaState;

    void Get(LuaState& L, int& value);
    void Get(LuaState& L, double& value);
    void Get(LuaState& L, float& value);
    void Get(LuaState& L, bool& value);
    void Get(LuaState& L, std::string& value);
    
    void GetGlobal(LuaState& lua, const std::string& name);

    template <typename T>
    T GetValue(LuaState& lua, const std::string& name)
    {
        lua::GetGlobal(lua, name);
        
        T value;
        lua::Get(lua, value);
        return value;

    }
}
