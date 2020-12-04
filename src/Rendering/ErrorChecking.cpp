
#include "ErrorChecking.h"
#include "System/open_gl.h"
#include "System/System.h"
#include <cstdio>

void ProcessGLErrors(int line, const char* error_context)
{
    bool print_backtrace = false;
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        System::Log("OpenGL Error: 0x%X - %s @ line %d\n", error, error_context, line);
        print_backtrace = true;
    }

//    if(print_backtrace)
//    {
//        System::DebugPrintBacktrace();
//        System::DebugBreakpoint();
//    }
}
