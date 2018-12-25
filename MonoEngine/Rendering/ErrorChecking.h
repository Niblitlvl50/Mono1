
#pragma once

void ProcessGLErrors(const char* error_context);

#define PROCESS_GL_ERRORS() \
    ProcessGLErrors(__PRETTY_FUNCTION__) 
