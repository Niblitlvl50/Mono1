/*
 *  SquareObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SquareObject.h"
#include <GLUT/glut.h>

#include "TextureFactory.h"

#include <string>

using namespace game;

SquareObject::SquareObject(float x, float y, float z)
	: mPosX(x),
	  mPosY(y),
	  mPosZ(z)
{
    mTexture = mono::CreateTexture("small_craft.png");
}

void SquareObject::Draw() const
{
    mTexture->Use();
    
	glPushMatrix();
	
	glTranslatef(mPosX, mPosY, mPosZ);
	glScalef(0.5f, 0.5f, 0.5f);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_QUADS);
	
    glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0f, -1.0f, 0.0f);
	
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0f, 1.0f, 0.0f);
	
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0f, 1.0f, 0.0f);
	
	glEnd();	
	
	EntityBase::Draw();

	glPopMatrix();
}

void SquareObject::Update(unsigned int delta)
{
    EntityBase::Update(delta);
}
