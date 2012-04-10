//
//  LuaTest.cpp
//  UnitTests
//
//  Created by Niblit on 2012-04-08.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest.h"
#include "LuaInterpreter.h"

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
    
    EXPECT_TRUE(foo.mFunctionCalled);
}


