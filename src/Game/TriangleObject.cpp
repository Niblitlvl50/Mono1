/*
 *  TriangleObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleObject.h"
#include <GLUT/glut.h>
#include "SquareObject.h"
#include "Texture.h"

using namespace game;

TriangleObject::TriangleObject()
    : mRotation(1.0f)
{
	AddChild(mono::IEntityPtr(new game::SquareObject(1.0f, 1.0f, 0.0f)));
	AddChild(mono::IEntityPtr(new game::SquareObject(-1.0f, -1.0f, 0.0f)));
}

void TriangleObject::Draw() const
{
    mono::Texture::Clear();
    
	glPushMatrix();
		
	glRotatef(mRotation, 0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glBegin(GL_TRIANGLES);
	
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
    
    glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
    
    glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	
	glEnd();	
	
	EntityBase::Draw();
	
	glPopMatrix();
}

void TriangleObject::Update(unsigned int delta)
{
	mRotation += 1.0f;
	if(mRotation > 360.0f)
		mRotation = 0.0f;
    
    EntityBase::Update(delta);
}
