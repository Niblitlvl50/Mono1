/*
 *  TriangleObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleObject.h"
#include "SysOpenGL.h"
#include "SquareObject.h"
#include "Texture.h"

using namespace game;

TriangleObject::TriangleObject()
{
    mPosition = Math::Vector2f(400.0f, 300.0f);
    mScale = 20.0f;
    mRotation = 1.0f;
    
    AddChild(mono::IEntityPtr(new game::SquareObject(1.0f, 1.0f)));
    AddChild(mono::IEntityPtr(new game::SquareObject(-1.0f, -1.0f)));
}

void TriangleObject::Draw() const
{
    mono::Texture::Clear();
        
    glColor3f(1.0f, 0.0f, 0.0f);	
    glBegin(GL_TRIANGLES);
	
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glEnd();
}

void TriangleObject::Update(unsigned int delta)
{
    mRotation += (delta * 0.1f);
    if(mRotation > 360.0f)
        mRotation -= 360.0f;
}
