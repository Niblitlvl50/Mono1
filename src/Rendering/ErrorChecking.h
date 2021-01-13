
#pragma once

void ProcessGLErrors(int line, const char* error_context);

#ifdef _WIN32
#define PROCESS_GL_ERRORS() \
    ProcessGLErrors(__LINE__, __FUNCSIG__)
#else
#define PROCESS_GL_ERRORS() \
    ProcessGLErrors(__LINE__, __PRETTY_FUNCTION__)
#endif
