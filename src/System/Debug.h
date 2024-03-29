
#pragma once

namespace debug
{
    void PrintStacktrace(int skip_frames, int n_frames);
    void LogAssert(const char* condition, const char* message, const char* file, int line);
    void DebugBreak();
}

#ifdef NDEBUG

#define MONO_ASSERT(condition) ((void)0)
#define MONO_ASSERT_MESSAGE(condition, message) ((void)0)

#else

/*
#ifdef __APPLE__
#define MONO_BREAKPOINT() __builtin_debugtrap()
#elif _MSC_VER
#define MONO_BREAKPOINT() debug::DebugBreak()
#else
#define MONO_BREAKPOINT() ((void)0)
#endif
*/

#define MONO_ASSERT(condition) \
    do { \
        while ( !(condition) ) { \
            debug::LogAssert(#condition, nullptr, __FILE__, __LINE__); \
            debug::DebugBreak(); \
            break; \
        } \
    } while (false)

#define MONO_ASSERT_MESSAGE(condition, message) \
    do { \
        while ( !(condition) ) { \
            debug::LogAssert(#condition, message, __FILE__, __LINE__); \
            debug::DebugBreak(); \
            break; \
        } \
    } while (false)

#endif
