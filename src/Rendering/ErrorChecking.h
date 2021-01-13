
#pragma once

void ProcessGLErrors(int line, const char* error_context);

#define PROCESS_GL_ERRORS() \
#ifdef _WIN32
    ProcessGLErrors(__LINE__, __FUNCSIG__)
#elif
    ProcessGLErrors(__LINE__, __PRETTY_FUNCTION__)
#endif
