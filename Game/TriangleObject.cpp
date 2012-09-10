/*
 *  TriangleObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleObject.h"
#include "SquareObject.h"
#include "SysOpenGL.h"
#include "Texture.h"

using namespace game;

namespace
{
    static const float vertex[] = { 0.0f, 0.0f }; 
}

TriangleObject::TriangleObject()
{
    mPosition = Math::Vector2f(400.0f, 300.0f);
    mScale = 20.0f;
    
    AddChild(mono::IEntityPtr(new game::SquareObject(-1.0f, -1.0f)));
    AddChild(mono::IEntityPtr(new game::SquareObject(1.0f, 1.0f)));
}

void TriangleObject::Draw(mono::IRenderer&) const
{
    mono::Texture::Clear();
    
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(5.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glDrawArrays(GL_POINTS, 0, 1);
    
    glDisableClientState(GL_VERTEX_ARRAY);
}

void TriangleObject::Update(unsigned int delta)
{
    mRotation += (delta * 0.1f);
    if(mRotation > 360.0f)
        mRotation -= 360.0f;
}
