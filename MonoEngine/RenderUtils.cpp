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
#include "Sprite.h"
#include "SysOpenGL.h"

void mono::DrawQuad(const math::Quad& quad, const mono::Color& color)
{
    mono::Texture::Clear();
    glColor4f(color.red, color.green, color.blue, color.alpha);
    
    const float vertices[] = { quad.mA.mX, quad.mA.mY,
                               quad.mB.mX, quad.mA.mY,
                               quad.mB.mX, quad.mB.mY,
                               quad.mA.mX, quad.mB.mY };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);        
}

void mono::DrawSprite(const mono::Sprite& sprite)
{
    static const float vertices[] = { -0.5f, -0.5f,
                                      -0.5f,  0.5f,
                                       0.5f,  0.5f,
                                       0.5f, -0.5f };
    
    static const unsigned short indices[] = { 0, 2, 1, 0, 3, 2 };    

    sprite.GetTexture()->Use();
    const math::Quad& quad = sprite.GetTextureCoords();
    const float coords[] = { quad.mA.mX, quad.mA.mY,
                             quad.mA.mX, quad.mB.mY,
                             quad.mB.mX, quad.mB.mY,
                             quad.mB.mX, quad.mA.mY };
    
    const mono::Color& color = sprite.GetShade();
    glColor4f(color.red, color.green, color.blue, color.alpha);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, coords);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


