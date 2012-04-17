//
//  OscillatingLine.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "OscillatingLine.h"
#include "SysOpenGL.h"
#include "Texture.h"

#include <cmath>

using namespace game;

OscillatingLine::OscillatingLine()
    : mDelta(0.0f)
{ }

void OscillatingLine::Draw() const
{
    const float radius = 50.0f;
    const float deltasin = sin(mDelta) * radius;
    const float deltacos = cos(mDelta) * radius;
    
    mono::Texture::Clear();
    
    glPushMatrix();
    
    glTranslated(400.0, 100.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glBegin(GL_POINTS);
    glVertex2f(deltasin, deltacos);
    glEnd();
    
    EntityBase::Draw();
    
    glPopMatrix();
}

void OscillatingLine::Update(unsigned int delta)
{
    mDelta += (delta * 0.001f);

    EntityBase::Update(delta);
}

