
#include "Debug.h"

#include "System/System.h"

#include <cstdlib>
#include <cstdio>

#ifdef __APPLE__
#include <execinfo.h>
#endif

void debug::PrintStacktrace(int skip_frames, int n_frames)
{
    if(n_frames <= skip_frames)
        return;

#ifdef __APPLE__

    constexpr int max_stack_frames = 64;
    if(n_frames > max_stack_frames)
        n_frames = max_stack_frames;

    void* callstack[max_stack_frames];
    const int frames = backtrace(callstack, n_frames);
    char** strs = backtrace_symbols(callstack, frames);
    for (int i = skip_frames; i < frames; ++i)
        System::Log("%s", strs[i]);
    
    free(strs);
#endif
}

void debug::LogAssert(const char* condition, const char* message, const char* file, int line)
{
    if(message)
    {
        System::Log("[%s(%d)] Assert! Condition: '%s' Message: '%s'.", file, line, condition, message);
    }
    else
    {
        System::Log("[%s(%d)] Assert! Condition: '%s'.", file, line, condition);
    }

    //debug::PrintStacktrace(2, 10); 
}

#ifdef __APPLE__

void debug::DebugBreak()
{
    __builtin_debugtrap();
}

#elif _WIN32

#include "Windows.h"

void debug::DebugBreak()
{
    ::DebugBreak();
}

#else

void debug::DebugBreak()
{
}

#endif

