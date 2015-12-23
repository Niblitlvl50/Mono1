//
//  CacoPlasma.hpp
//  MonoOSX
//
//  Created by Niklas Damberg on 23/12/15.
//
//

#pragma once

#include "BaseWeapon.h"

namespace game
{
    class CacoPlasma : public BaseWeapon
    {
    public:

        CacoPlasma(mono::EventHandler& eventHandler);

    private:
        
        virtual void DoFire(const math::Vector2f& position, float direction) const;
        virtual int RoundsPerSecond() const;
    };
}

