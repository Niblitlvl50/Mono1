
#include "Debug.h"

#include "System/System.h"

#include <cstdlib>
#include <cstdio>

#ifdef __APPLE__
#include <execinfo.h>
#endif

void debug::PrintStacktrace()
{
#ifdef __APPLE__
    void* callstack[128];
    const int frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    for (int i = 0; i < frames; ++i)
        System::Log("%s\n", strs[i]);
    
    free(strs);
#endif
}