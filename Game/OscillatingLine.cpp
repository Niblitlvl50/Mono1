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
#include "IRenderer.h"

#include <string>
#include <cmath>

using namespace game;

namespace
{
    static const float vertex[] = { 0.0f, 0.0f };
}

OscillatingLine::OscillatingLine()
    : mBase(400.0f, 100.0f),
      mDelta(0.0f)
{
    mScale = 3.0f;
}

void OscillatingLine::Draw(mono::IRenderer& renderer) const
{    
    mono::Texture::Clear();
        
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(4.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glDrawArrays(GL_POINTS, 0, 1);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    Color color = { 0.0f, 0.0f, 0.0f, 1.0f };
    renderer.DrawText("dot...", mPosition + math::Vector2f(0.0f, 5.0f), true, color);
}

void OscillatingLine::Update(unsigned int delta)
{
    mDelta += (delta * 0.001f);

    const float deltasin = std::sin(mDelta) * 50.0f;
    const float deltacos = std::cos(mDelta) * 50.0f;
    
    mPosition = mBase + math::Vector2f(deltasin, deltacos);
}

