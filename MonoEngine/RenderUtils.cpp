//
//  RenderUtils.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "RenderUtils.h"
#include "Quad.h"
#include "Texture.h"
#include "SysOpenGL.h"

void mono::DrawQuad(const Math::Quad& quad)
{
    mono::Texture::Clear();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    const float vertices[] = { quad.mA.mX, quad.mA.mY,
        quad.mB.mX, quad.mA.mY, 
        quad.mB.mX, quad.mB.mY,
        quad.mA.mX, quad.mB.mY };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);        
}
