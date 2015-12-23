//
//  RocketLauncher.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "BaseWeapon.h"

namespace game
{
    class RocketLauncher : public BaseWeapon
    {
    public:

        RocketLauncher(mono::EventHandler& eventHandler);

    private:
        
        virtual int RoundsPerSecond() const;
        virtual void DoFire(const math::Vector2f& position, float direction) const;
    };
}
