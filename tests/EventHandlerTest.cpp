
#include "gtest/gtest.h"
#include "EventHandler/EventHandler.h"
#include "System/System.h"

#include <functional>
#include <cstdint>

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
            return false;
        }
        
        bool OnAnohterEvent(const TestEvent2& event)
        {
            receivedAnotherEvent = true;
            return false;
        }
        
        bool receivedEvent = false;
        bool receivedAnotherEvent = false;
    };

    struct ScopedTimer
    {
        ScopedTimer(uint32_t& out_diff_time)
            : m_before_time(System::GetMilliseconds())
            , m_out_diff_time(out_diff_time)
        { }

        ~ScopedTimer()
        {
            m_out_diff_time = System::GetMilliseconds() - m_before_time;
        }

        const uint32_t m_before_time;
        uint32_t& m_out_diff_time;
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

TEST(EventHandlerTest, stress_test)
{
    mono::EventHandler handler;

    constexpr int n_objects = 5000;
    TestClass receiving_objects[n_objects];
    mono::EventToken<TestEvent1> testevent_token_1[n_objects];
    mono::EventToken<TestEvent2> testevent_token_2[n_objects];

    uint32_t add_listener_diff = 0;
    {
        ScopedTimer scope_timer(add_listener_diff);

        for(int index = 0; index < n_objects; ++index)
        {
            TestClass& object = receiving_objects[index];

            std::function<bool (const TestEvent1&)> func1 = std::bind(&TestClass::OnEventFunc, &object, _1);
            std::function<bool (const TestEvent2&)> func2 = std::bind(&TestClass::OnAnohterEvent, &object, _1);

            testevent_token_1[index] = handler.AddListener(func1);
            testevent_token_2[index] = handler.AddListener(func2);
        }
    }

    uint32_t dispatch_diff = 0;
    {
        ScopedTimer scope_timer(dispatch_diff);
        handler.DispatchEvent(TestEvent1());
        handler.DispatchEvent(TestEvent2());
    }

    uint32_t remove_diff = 0;
    {
        ScopedTimer scope_timer(remove_diff);

        for(int index = n_objects; index > 0; --index)
        {
            handler.RemoveListener(testevent_token_1[index -1]);
            handler.RemoveListener(testevent_token_2[index -1]);
        }
    }

    for(int index = 0; index < n_objects; ++index)
    {
        TestClass& object = receiving_objects[index];
        ASSERT_TRUE(object.receivedEvent);
        ASSERT_TRUE(object.receivedAnotherEvent);
    }

    std::printf("---------------------\n");
    std::printf("add: %u ms, dispatch: %u ms, remove: %u ms\n", add_listener_diff, dispatch_diff, remove_diff);
    std::printf("---------------------\n");
}
