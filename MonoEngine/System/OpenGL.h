
#pragma once

#ifdef __IPHONEOS__
    #include "SDL_opengles2.h"
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include "SDL_opengl.h"
#endif

void ProcessGLErrors(const char* error_context);

#define PROCESS_GL_ERRORS() \
    ProcessGLErrors(__PRETTY_FUNCTION__) 
