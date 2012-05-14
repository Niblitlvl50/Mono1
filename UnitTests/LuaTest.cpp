//
//  LuaTest.cpp
//  UnitTests
//
//  Created by Niblit on 2012-04-08.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest.h"
#include "LuaInterpreter.h"
#include "LuaState.h"
#include "LuaFunctions.h"

#include <stdexcept>

namespace
{
    bool functionCalled = false;
    
    int FunctionToCallFromLua(lua_State* L)
    {
        std::cout << "Function called!" << std::endl;
        functionCalled = true;
        
        return 0;
    }
}

/*
TEST(LuaTest, BasicFunction)
{
    mono::LuaInterpreter lua;
    EXPECT_NO_THROW(lua.RegisterFunction("FunctionToCallFromLua", FunctionToCallFromLua));

    const std::string luaScript = "io.write('Running script \\n') FunctionToCallFromLua(1, 2, 3)";
    lua.ExecuteString(luaScript);
    
    EXPECT_TRUE(functionCalled);
}
*/

namespace
{
    struct Object
    {
        Object()
            : mFunctionCalled(false)
        { }
        int Function(lua_State* L)
        {
            mFunctionCalled = true;
            return 0;
        }
        
        bool mFunctionCalled;
    };
}

TEST(LuaTest, MemberFunction)
{
    using namespace std::tr1::placeholders;
    
    Object foo;
    
    mono::LuaInterpreter lua;
    lua.RegisterFunction("MemberFunc", std::tr1::bind(&Object::Function, foo, _1));
    
    const std::string luaScript = "io.write('Running script \\n') MemberFunc(1, 2, 3)";
    lua.ExecuteString(luaScript);
    
    //EXPECT_TRUE(foo.mFunctionCalled);
}

TEST(LuaTest, LoadFileAndFetchData)
{
    const std::string file = "luaconfig.lua";
    lua::LuaState config(file);
    
    const int width = lua::GetValue<int>(config, "width");
    EXPECT_EQ(400, width);
    
    const double pi = lua::GetValue<double>(config, "pi");
    EXPECT_DOUBLE_EQ(3.14, pi);
    
    const float gravity = lua::GetValue<float>(config, "gravity");
    EXPECT_FLOAT_EQ(9.81f, gravity);
    
    const bool boolValue = lua::GetValue<bool>(config, "isThisTrue");
    EXPECT_FALSE(boolValue);
    
    const std::string sprite = lua::GetValue<std::string>(config, "sprite");
    EXPECT_STRCASEEQ("ryu.png", sprite.c_str());
}

TEST(LuaTest, GetWrongValue_ShouldThrow)
{
    const std::string file = "luaconfig.lua";
    lua::LuaState config(file);
    
    try
    {
        lua::GetValue<bool>(config, "sprite");
        ADD_FAILURE();
    }
    catch(const std::runtime_error& e)
    {
    
    }
    
    //EXPECT_THROW(lua::GetValue<bool>(config, "sprite"), std::runtime_error);
    //EXPECT_THROW(lua::GetValue<int>(config, "pi"), std::runtime_error);
    //EXPECT_THROW(config.GetValue<std::string>("gravity"), std::runtime_error);
    //EXPECT_THROW(config.GetValue<double>("width"), std::runtime_error);
    //EXPECT_THROW(lua::GetValue<float>(config, "isThisTrue"), std::runtime_error);
}

TEST(LuaTest, GetLuaTableAsVector)
{
    const std::string file = "luaconfig.lua";
    lua::LuaState config(file);
    
    const lua::IntTable intValues = lua::GetTable<int>(config, "intTable");
    EXPECT_EQ(5, intValues.size());
    
    EXPECT_EQ(10, intValues.at(0));
    EXPECT_EQ(20, intValues.at(1));
    EXPECT_EQ(30, intValues.at(2));
    EXPECT_EQ(40, intValues.at(3));
    EXPECT_EQ(50, intValues.at(4));

    const lua::StringTable stringValues = lua::GetTable<std::string>(config, "stringTable");
    EXPECT_EQ(4, stringValues.size());
    
    EXPECT_STREQ("hello", stringValues.at(0).c_str());
    EXPECT_STREQ("im", stringValues.at(1).c_str());
    EXPECT_STREQ("a", stringValues.at(2).c_str());
    EXPECT_STREQ("table!", stringValues.at(3).c_str());

    const lua::BoolTable boolValues = lua::GetTable<bool>(config, "boolTable");
    EXPECT_EQ(3, boolValues.size());
    
    EXPECT_TRUE(boolValues.at(0));
    EXPECT_FALSE(boolValues.at(1));
    EXPECT_TRUE(boolValues.at(2));    

    const lua::FloatTable floatValues = lua::GetTable<float>(config, "floatTable");
    EXPECT_EQ(2, floatValues.size());
    
    EXPECT_FLOAT_EQ(99.9f, floatValues.at(0));
    EXPECT_FLOAT_EQ(45.2f, floatValues.at(1));

    const lua::DoubleTable doubleValues = lua::GetTable<double>(config, "doubleTable");
    EXPECT_EQ(2, doubleValues.size());
    
    EXPECT_DOUBLE_EQ(15.2, doubleValues.at(0));
    EXPECT_DOUBLE_EQ(129.0, doubleValues.at(1));
}

TEST(LuaTest, GetLuaMapTable)
{
    const std::string file = "luaconfig.lua";
    lua::LuaState config(file);
    
    const lua::MapIntTable intMap = lua::GetTableMap<std::string, int>(config, "definedAnimations");
    EXPECT_EQ(2, intMap.size());
    
    const lua::IntTable& first = intMap.find("Run")->second;
    EXPECT_EQ(4, first.size());
    
    EXPECT_EQ(0, first.at(0));
    EXPECT_EQ(1, first.at(1));
    EXPECT_EQ(2, first.at(2));
    EXPECT_EQ(3, first.at(3));
    
    const lua::IntTable& second = intMap.find("Fight")->second;
    EXPECT_EQ(3, second.size());

    EXPECT_EQ(4, second.at(0));
    EXPECT_EQ(5, second.at(1));
    EXPECT_EQ(6, second.at(2));
    
    const lua::MapIntIntTable table = lua::GetTableMap<int, int>(config, "anotherTable");
    EXPECT_EQ(2, table.size());
    
    const lua::IntTable& one = table.find(7)->second;
    EXPECT_EQ(3, one.size());
    
    EXPECT_EQ(123, one.at(0));
    EXPECT_EQ(321, one.at(1));
    EXPECT_EQ(777, one.at(2));
    
    const lua::IntTable& two = table.find(34)->second;
    EXPECT_EQ(4, two.size());

    EXPECT_EQ(99, two.at(0));
    EXPECT_EQ(66, two.at(1));
    EXPECT_EQ(22, two.at(2));
    EXPECT_EQ(00, two.at(3));
}









