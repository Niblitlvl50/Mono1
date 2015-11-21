//
//  CubeSwarm.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 24/03/15.
//
//

#include "CubeSwarm.h"
#include "IRenderer.h"
#include "Color.h"
#include "Utils.h"

#include <functional>

using namespace game;


CubeSwarm::CubeSwarm()
    : mColor(0.0f, 0.5f, 0.5f)
{
    mCubes.resize(100);

    auto func = std::bind(&CubeSwarm::GenerateCube, this);
    std::generate(mCubes.begin(), mCubes.end(), func);
}

void CubeSwarm::Draw(mono::IRenderer& renderer) const
{
    const mono::Color::RGBA color = mono::Color::ToRGBA(mColor);

    for(const Cube& cube : mCubes)
        renderer.DrawQuad(cube.quad, color, 2.0f);
}

void CubeSwarm::Update(unsigned int delta)
{
    for(Cube& cube : mCubes)
    {
        const float add_x = mono::Random() * 2.0 - 1.0;
        const float add_y = mono::Random() * 2.0 - 1.0;
        const math::Vector2f add(add_x, add_y);

        cube.quad.mA += add;
        cube.quad.mB += add;
    }

    mColor.hue += delta * 0.0005f;
    if(mColor.hue > 1.0f)
        mColor.hue = 0.0f;
}

CubeSwarm::Cube CubeSwarm::GenerateCube()
{
    const float random_x = mono::Random() * 100 - 50;
    const float random_y = mono::Random() * 100 - 50;
    const float random_size = mono::Random() * 10;

    return CubeSwarm::Cube(math::Vector2f(random_x, random_y), random_size);
}
