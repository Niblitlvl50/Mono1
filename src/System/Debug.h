
#pragma once

namespace debug
{
    void PrintStacktrace(int skip_frames, int n_frames);
}

#ifdef NDEBUG

#define MONO_ASSERT(condition) ((void)0)
#define MONO_ASSERT_MESSAGE(condition, message) ((void)0)

#else

#include "System/System.h"

#ifdef __APPLE__
#define MONO_BREAKPOINT() __builtin_debugtrap()
#else
#define MONO_BREAKPOINT() ((void)0)
#endif

#define MONO_ASSERT(condition) \
    do { \
        while ( !(condition) ) { \
            System::Log("Assert failed! Condition: '%s'.", #condition); \
            MONO_BREAKPOINT(); \
            break; \
        } \
    } while (false)

#define MONO_ASSERT_MESSAGE(condition, message) \
    do { \
        while ( !(condition) ) { \
            System::Log("Assert failed! Condition: '%s' Message: '%s'.", #condition, message); \
            MONO_BREAKPOINT(); \
            break; \
        } \
    } while (false)

#endif
