//
//  EventHandlerTest.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 20/12/13.
//
//

#include "gtest/gtest.h"
#include "EventHandler.h"

#include <functional>

using namespace std::placeholders;

namespace
{
    struct TestEvent1
    { };
    
    struct TestEvent2
    { };
    
    struct TestClass
    {
        void OnEventFunc(const TestEvent1& event)
        {
            receivedEvent = true;
        }
        
        void OnAnohterEvent(const TestEvent2& event)
        {
            receivedAnotherEvent = true;
        }
        
        bool receivedEvent = false;
        bool receivedAnotherEvent = false;
    };
}

TEST(EventHandlerTest, RegisterListener)
{
    TestClass object;
    
    std::function<void (const TestEvent1&)> func1 = std::bind(&TestClass::OnEventFunc, &object, _1);
    std::function<void (const TestEvent2&)> func2 = std::bind(&TestClass::OnAnohterEvent, &object, _1);

    mono::MyEventHandler handler;
    handler.AddListener(func1);
    handler.AddListener(func2);
    
    EXPECT_FALSE(object.receivedEvent);
    EXPECT_FALSE(object.receivedAnotherEvent);
    
    TestEvent1 event1;
    handler.DispatchEvent(event1);
    
    EXPECT_TRUE(object.receivedEvent);
    EXPECT_FALSE(object.receivedAnotherEvent);
    
    TestEvent2 event2;
    handler.DispatchEvent(event2);
    
    EXPECT_TRUE(object.receivedAnotherEvent);
}


