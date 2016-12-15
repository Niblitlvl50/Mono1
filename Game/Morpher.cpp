//
//  Morpher.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/15.
//
//

#include "Morpher.h"
#include "IRenderer.h"
#include "Color.h"
#include "FontIds.h"

using namespace game;

Morpher::Morpher()
{
    mPosition = math::Vector(-100, 50);

    m_shape1 = { math::Vector(-10, -10),
                 math::Vector(-10, 10),
                 math::Vector(10, 10),
                 math::Vector(10, -10),
                 math::Vector(-10, -10) };

    m_shape2 = { math::Vector(-15, -5),
                 math::Vector(-13, 26),
                 math::Vector(10, 29),
                 math::Vector(7, -14),
                 math::Vector(-15, -5) };

    m_morphGrade = 0.0;

    m_direction = true;
    m_counter = 0;
}

void Morpher::Draw(mono::IRenderer& renderer) const
{
    constexpr mono::Color::RGBA color(0, 1, 1);
    renderer.DrawShape(m_shape1, m_shape2, m_morphGrade, color);
    renderer.DrawText(FontId::LARGE, "Here", math::zeroVec, true, color);
}

void Morpher::Update(unsigned int delta)
{
    m_counter += m_direction ? delta : -delta;

    constexpr float duration = 3000;
    m_morphGrade = m_counter / duration;

    if(m_counter > duration)
        m_direction = false;
    else if(m_counter < 0)
        m_direction = true;
}
