//
//  OscillatingLine.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <GLUT/glut.h>
#include <cmath>

#include "OscillatingLine.h"
#include "Texture.h"

using namespace game;

OscillatingLine::OscillatingLine()
    : mDelta(0.0f)
{ }

void OscillatingLine::Draw() const
{
    const float deltasin = sin(mDelta);
    const float deltacos = cos(mDelta);
    
    mono::Texture::Clear();
    
    glPushMatrix();
    
    glTranslated(0.0, -4.0, 0.0);
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

