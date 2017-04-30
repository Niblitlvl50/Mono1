
#include "gtest/gtest.h"
#include "EventHandler/EventHandler.h"

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
        bool OnEventFunc(const TestEvent1& event)
        {
            receivedEvent = true;
            return true;
        }
        
        bool OnAnohterEvent(const TestEvent2& event)
        {
            receivedAnotherEvent = true;
            return true;
        }
        
        bool receivedEvent = false;
        bool receivedAnotherEvent = false;
    };
}

TEST(EventHandlerTest, RegisterListener)
{
    TestClass object;
    
    std::function<bool (const TestEvent1&)> func1 = std::bind(&TestClass::OnEventFunc, &object, _1);
    std::function<bool (const TestEvent2&)> func2 = std::bind(&TestClass::OnAnohterEvent, &object, _1);

    mono::EventHandler handler;
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


