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
    
    template <typename T>
    void GetLuaMap(lua::LuaState& L, std::map< std::string, std::vector<T> >& values)
    {
        const bool result = (lua_istable(L, -1) != 0);
        if(!result)
            throw std::runtime_error("Value is not a table");

        lua_pushnil(L);
        while(lua_next(L, -2) != 0)
        {
            const std::string key = lua_tostring(L, -2);
            std::vector<T> table;
            GetLuaTable(L, table);
            
            values[key] = table;
            
            lua_pop(L, 1);
        }
    }
    
    template <typename T>
    void GetLuaIntMap(lua::LuaState& L, std::map<int, std::vector<T> >& values)
    {
        const bool result = (lua_istable(L, -1) != 0);
        if(!result)
            throw std::runtime_error("Value is not a table");
        
        lua_pushnil(L);
        while(lua_next(L, -2) != 0)
        {
            const int key = lua_tointeger(L, -2);
            std::vector<T> table;
            GetLuaTable(L, table);
            
            values[key] = table;
            
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

void lua::GetTable(LuaState& L, IntTable& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, DoubleTable& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, FloatTable& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, BoolTable& values)
{
    GetLuaTable(L, values);    
}
void lua::GetTable(LuaState& L, StringTable& values)
{
    GetLuaTable(L, values);    
}

void lua::GetMap(LuaState& L, MapIntTable& values)
{
    GetLuaMap(L, values);
}
void lua::GetMap(LuaState& L, MapDoubleTable& values)
{
    GetLuaMap(L, values);
}
void lua::GetMap(LuaState& L, MapFloatTable& values)
{
    GetLuaMap(L, values);
}
void lua::GetMap(LuaState& L, MapBoolTable& values)
{
    GetLuaMap(L, values);
}
void lua::GetMap(LuaState& L, MapStringTable& values)
{
    GetLuaMap(L, values);
}

void lua::GetMap(LuaState& L, MapIntIntTable& values)
{
    GetLuaIntMap(L, values);
}
void lua::GetMap(LuaState& L, MapIntDoubleTable& values)
{
    GetLuaIntMap(L, values);
}
void lua::GetMap(LuaState& L, MapIntFloatTable& values)
{
    GetLuaIntMap(L, values);
}
void lua::GetMap(LuaState& L, MapIntBoolTable& values)
{
    GetLuaIntMap(L, values);
}
void lua::GetMap(LuaState& L, MapIntStringTable& values)
{
    GetLuaIntMap(L, values);
}


void lua::GetGlobal(LuaState& lua, const std::string& name)
{
    lua_getglobal(lua, name.c_str());
}

