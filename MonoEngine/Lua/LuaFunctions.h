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
#include <map>


namespace lua
{
    struct LuaState;
    
    typedef std::vector<int> IntTable;
    typedef std::vector<double> DoubleTable;
    typedef std::vector<float> FloatTable;
    typedef std::vector<bool> BoolTable;
    typedef std::vector<std::string> StringTable;
    
    typedef std::map<std::string, IntTable> MapIntTable;
    typedef std::map<std::string, DoubleTable> MapDoubleTable;
    typedef std::map<std::string, FloatTable> MapFloatTable;
    typedef std::map<std::string, BoolTable> MapBoolTable;
    typedef std::map<std::string, StringTable> MapStringTable;

    typedef std::map<int, IntTable> MapIntIntTable;
    typedef std::map<int, DoubleTable> MapIntDoubleTable;
    typedef std::map<int, FloatTable> MapIntFloatTable;
    typedef std::map<int, BoolTable> MapIntBoolTable;
    typedef std::map<int, StringTable> MapIntStringTable;

    void Get(LuaState& L, int& value);
    void Get(LuaState& L, double& value);
    void Get(LuaState& L, float& value);
    void Get(LuaState& L, bool& value);
    void Get(LuaState& L, std::string& value);
    
    void GetTable(LuaState& L, IntTable& values);
    void GetTable(LuaState& L, DoubleTable& values);
    void GetTable(LuaState& L, FloatTable& values);
    void GetTable(LuaState& L, BoolTable& values);
    void GetTable(LuaState& L, StringTable& values);
    
    void GetMap(LuaState& L, MapIntTable& values);
    void GetMap(LuaState& L, MapDoubleTable& values);
    void GetMap(LuaState& L, MapFloatTable& values);
    void GetMap(LuaState& L, MapBoolTable& values);
    void GetMap(LuaState& L, MapStringTable& values);

    void GetMap(LuaState& L, MapIntIntTable& values);
    void GetMap(LuaState& L, MapIntDoubleTable& values);
    void GetMap(LuaState& L, MapIntFloatTable& values);
    void GetMap(LuaState& L, MapIntBoolTable& values);
    void GetMap(LuaState& L, MapIntStringTable& values);
    
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
    
    template <typename K, typename T>
    std::map<K, std::vector<T> > GetTableMap(LuaState& lua, const std::string& name)
    {
        lua::GetGlobal(lua, name);
        std::map<K, std::vector<T> > values;

        try
        {
            lua::GetMap(lua, values);
        }
        catch(...)
        { }
        
        return values;
    }
}






