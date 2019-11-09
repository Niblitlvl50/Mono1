
#include "ErrorChecking.h"
#include "System/open_gl.h"
#include "System/System.h"
#include <cstdio>

void ProcessGLErrors(const char* error_context)
{
    bool print_backtrace = false;
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        System::Log("OpenGL Error: 0x%X - context: %s\n", error, error_context);
        print_backtrace = true;
    }

//    if(print_backtrace)
//    {
//        System::DebugPrintBacktrace();
//        System::DebugBreakpoint();
//    }
}
