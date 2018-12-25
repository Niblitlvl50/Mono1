
#include "ErrorChecking.h"
#include "System/open_gl.h"
#include <cstdio>

void ProcessGLErrors(const char* error_context)
{
    bool print_backtrace = false;
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::printf("OpenGL Error: 0x%X - context: %s\n", error, error_context);
        print_backtrace = true;
    }

//    if(print_backtrace)
//    {
//        System::DebugPrintBacktrace();
//        System::DebugBreakpoint();
//    }
}
