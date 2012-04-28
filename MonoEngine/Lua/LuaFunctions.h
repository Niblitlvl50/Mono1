//
//  LuaHelpers.h
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>
#include <vector>


namespace lua
{
    struct LuaState;

    void Get(LuaState& L, int& value);
    void Get(LuaState& L, double& value);
    void Get(LuaState& L, float& value);
    void Get(LuaState& L, bool& value);
    void Get(LuaState& L, std::string& value);
    
    void GetTable(LuaState& L, std::vector<int>& values);
    void GetTable(LuaState& L, std::vector<double>& values);
    void GetTable(LuaState& L, std::vector<float>& values);
    void GetTable(LuaState& L, std::vector<bool>& values);
    void GetTable(LuaState& L, std::vector<std::string>& values);
    
    void GetGlobal(LuaState& lua, const std::string& name);

    template <typename T>
    T GetValue(LuaState& lua, const std::string& name)
    {
        lua::GetGlobal(lua, name);
        
        T value;
        lua::Get(lua, value);
        
        return value;
    }
    
    template <typename T>
    std::vector<T> GetTable(LuaState& lua, const std::string& name)
    {
        lua::GetGlobal(lua, name);
        
        std::vector<T> values;
        lua::GetTable(lua, values);
        
        return values;
    }
}
