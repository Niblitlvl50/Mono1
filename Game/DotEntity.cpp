//
//  OscillatingLine.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "DotEntity.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Matrix.h"

#include <cmath>

using namespace game;

DotEntity::DotEntity()
    : mBase(400.0f, 400.0f),
      mDelta(0.0f)
{
    mScale = math::Vector2f(3.0f, 3.0f);
}

void DotEntity::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0) };
    constexpr mono::Color::RGBA color(0.0, 0.0, 0.0);
    constexpr float size = 4.0f;

    constexpr math::Vector2f text_offset(0.0f, 5.0f);
    const math::Vector2f& global_pos = math::Transform(Transformation(), text_offset);

    renderer.DrawPoints(points, color, size);
    renderer.DrawText("dot...", global_pos, true, color);
}

void DotEntity::Update(unsigned int delta)
{
    mDelta += (delta * 0.001f);

    const float deltasin = std::sin(mDelta) * 50.0f;
    const float deltacos = std::cos(mDelta) * 50.0f;
    
    mPosition = mBase + math::Vector2f(deltasin, deltacos);
}

