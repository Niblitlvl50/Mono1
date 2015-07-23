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

#include <functional>

using namespace game;


CubeSwarm::CubeSwarm()
{
    mCubes.resize(100);

    auto func = std::bind(&CubeSwarm::GenerateCube, this);
    std::generate(mCubes.begin(), mCubes.end(), func);
}

void CubeSwarm::Draw(mono::IRenderer& renderer) const
{
    const mono::Color color(1, 0, 0, 1);

    for(const Cube& cube : mCubes)
        renderer.DrawQuad(cube.quad, color, 1.0f);
}

void CubeSwarm::Update(unsigned int delta)
{

}

CubeSwarm::Cube CubeSwarm::GenerateCube()
{
    return CubeSwarm::Cube();
}
