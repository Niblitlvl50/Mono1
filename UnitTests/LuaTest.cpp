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

TEST(LuaTest, BasicFunction)
{
    mono::LuaInterpreter lua;
    EXPECT_NO_THROW(lua.RegisterFunction("FunctionToCallFromLua", FunctionToCallFromLua));

    const std::string luaScript = "io.write('Running script \\n') FunctionToCallFromLua(1, 2, 3)";
    lua.ExecuteString(luaScript);
    
    EXPECT_TRUE(functionCalled);
}

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




