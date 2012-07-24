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
{
    mPosition = Math::Vector2f(400.0f, 100.0f);
}

void OscillatingLine::Draw(mono::IRenderer&) const
{
    const float radius = 50.0f;
    const float deltasin = std::sin(mDelta) * radius;
    const float deltacos = std::cos(mDelta) * radius;
    
    mono::Texture::Clear();
        
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glBegin(GL_POINTS);
    glVertex2f(deltasin, deltacos);
    glEnd();
}

void OscillatingLine::Update(unsigned int delta)
{
    mDelta += (delta * 0.001f);
}

