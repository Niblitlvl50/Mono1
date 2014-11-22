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
#include "Texture.h"
#include "RenderUtils.h"

using namespace game;

TriangleObject::TriangleObject()
{
    mPosition = math::Vector2f(0.0f, -300.0f);
    mScale = math::Vector2f(20.0f, 20.0f);
    
    AddChild(std::make_shared<SquareObject>(-1, -1));
    AddChild(std::make_shared<SquareObject>(1, 1));
}

void TriangleObject::Draw(mono::IRenderer&) const
{
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0) };
    const mono::Color color(0.0, 0.0, 0.0, 1.0);
    const float size = 5.0f;

    mono::Texture::Clear();
    mono::DrawPoints(points, color, size);    
}

void TriangleObject::Update(unsigned int delta)
{
    mRotation += (delta * 0.1f);
    if(mRotation > 360.0f)
        mRotation -= 360.0f;
}
