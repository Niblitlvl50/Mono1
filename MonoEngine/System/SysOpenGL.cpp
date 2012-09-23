//
//  SysOpenGL.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SysOpenGL.h"

using namespace OGL;

OGLPushPopMatrix::OGLPushPopMatrix()
{
    glPushMatrix();
}

OGLPushPopMatrix::~OGLPushPopMatrix()
{
    glPopMatrix();
}

void OGL::glOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
#ifdef __IPHONEOS__
    ::glOrthof(left, right, bottom, top, zNear, zFar);
#else
    ::glOrtho(left, right, bottom, top, zNear, zFar);
#endif
}
