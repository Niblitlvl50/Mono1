//
//  LuaHelpers.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LuaFunctions.h"
#include "LuaState.h"
#include <stdexcept>

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

namespace
{
    
    template <typename T>
    void GetLuaTable(lua::LuaState& L, std::vector<T>& values)
    {
        const bool result = (lua_istable(L, -1) != 0);
        if(!result)
            throw std::runtime_error("Value is not a table");
        
        try
        {
            // Lua tables starts from index 1.
            int index = 1;
            
            while(true)
            {
                lua_pushinteger(L, index);
                lua_gettable(L, -2);
                
                T value;
                lua::Get(L, value);
                
                values.push_back(value);
                
                lua_pop(L, 1);
                
                ++index;
            }
        }
        catch(const std::runtime_error& e)
        {
            lua_pop(L, 1);
        }
    }
    
}


void lua::Get(LuaState& L, int& value)
{
    const bool result = (lua_isnumber(L, -1) != 0);
    if(!result)
        throw std::runtime_error("Value is not a integer");
    
    value = lua_tointeger(L, -1);
}

void lua::Get(LuaState& L, double& value)
{
    const bool result = (lua_isnumber(L, -1) != 0);
    if(!result)
        throw std::runtime_error("Value is not a double");
    
    value = lua_tonumber(L, -1);
}

void lua::Get(LuaState& L, float& value)
{
    const bool result = (lua_isnumber(L, -1) != 0);
    if(!result)
        throw std::runtime_error("Value is not a float");
    
    value = static_cast<float>(lua_tonumber(L, -1));
}

void lua::Get(LuaState& L, bool& value)
{
    const bool result = (lua_isboolean(L, -1) != 0);
    if(!result)
        throw std::runtime_error("Value is not a boolean");
    
    value = lua_toboolean(L, -1);
}

void lua::Get(LuaState& L, std::string& value)
{
    const bool result = (lua_isstring(L, -1) != 0);
    if(!result)
        throw std::runtime_error("Value is not a string");
    
    value = lua_tostring(L, -1);
}

void lua::GetTable(LuaState& L, std::vector<int>& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, std::vector<double>& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, std::vector<float>& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, std::vector<bool>& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, std::vector<std::string>& values)
{
    GetLuaTable(L, values);    
}


void lua::GetGlobal(LuaState& lua, const std::string& name)
{
    lua_getglobal(lua, name.c_str());
}

