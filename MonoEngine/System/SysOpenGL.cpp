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
