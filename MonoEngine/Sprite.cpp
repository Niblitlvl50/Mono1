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
                                      -1.0f,  1.0f,
                                       1.0f,  1.0f,
                                       1.0f, -1.0f };

    static const float texCoords[] = { 0.0f, 0.0f, 
                                       0.0f, 1.0f, 
                                       1.0f, 1.0f,
                                       1.0f, 0.0f };

    static const unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };
}


Sprite::Sprite(const std::string& file, float xscale, float yscale)
    : mXScale(xscale),
      mYScale(yscale)
{
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
