//
//  OscillatingLine.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "OscillatingLine.h"
#include "IRenderer.h"
#include "Color.h"
#include "RenderUtils.h"

#include <string>
#include <cmath>

using namespace game;

OscillatingLine::OscillatingLine()
    : mBase(400.0f, 100.0f),
      mDelta(0.0f)
{
    mScale = math::Vector2f(3.0f, 3.0f);
}

void OscillatingLine::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0) };
    const mono::Color color(0.0, 0.0, 0.0, 1.0);
    const float size = 4.0f;

    mono::DrawPoints(points, color, size);
    renderer.DrawText("dot...", mPosition + math::Vector2f(0.0f, 5.0f), true, color);
}

void OscillatingLine::Update(unsigned int delta)
{
    mDelta += (delta * 0.001f);

    const float deltasin = std::sin(mDelta) * 50.0f;
    const float deltacos = std::cos(mDelta) * 50.0f;
    
    mPosition = mBase + math::Vector2f(deltasin, deltacos);
}

