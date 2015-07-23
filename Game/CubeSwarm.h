//
//  CubeSwarm.h
//  MonoOSX
//
//  Created by Niklas Damberg on 24/03/15.
//
//

#pragma once

#include "EntityBase.h"
#include "Quad.h"
#include <vector>

namespace game
{
    class CubeSwarm : public mono::EntityBase
    {
    public:

        CubeSwarm();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        struct Cube
        {
            math::Quad quad;
            float rotation;
        };

        Cube GenerateCube();

        std::vector<Cube> mCubes;
    };
}
