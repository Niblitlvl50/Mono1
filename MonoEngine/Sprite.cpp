//
//  Sprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Sprite.h"
#include "TextureFactory.h"
#include "ITexture.h"
#include "SysOpenGL.h"

#include <map>

using namespace mono;

namespace
{
    static const float vertices[] = { -1.0f, -1.0f,
                                       1.0f, -1.0f,
                                       1.0f,  1.0f,
        
                                      -1.0f, -1.0f,
                                      -1.0f,  1.0f,
                                       1.0f,  1.0f };
    
    // This is only valid if all textures cover all of the surface. So its probably not valid in the long
    // run when more than one texture is on the same sheet.
    static const float texCoords[] = { 0.0f, 0.0f, 
                                       1.0f, 0.0f, 
                                       1.0f, 1.0f,
        
                                       0.0f, 0.0f,
                                       0.0f, 1.0f,
                                       1.0f, 1.0f };
}


Sprite::Sprite(const std::string& file, float xscale, float yscale)
    : mXScale(xscale),
      mYScale(yscale)
{
    //const std::map<std::string, std::string> configmap = mono::LuaConfig(file);
    mTexture = mono::CreateTexture(file);
}

void Sprite::DrawAt(float x, float y) const
{
    mTexture->Use();
    
	glPushMatrix();
	
	glTranslatef(x, y, 0.0f);
	glScalef(mXScale, mYScale, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
	    
	glPopMatrix();
}
