
#pragma once

void ProcessGLErrors(int line, const char* error_context);

#define PROCESS_GL_ERRORS() \
    ProcessGLErrors(__LINE__, __PRETTY_FUNCTION__) 
