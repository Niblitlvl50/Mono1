//
//  RocketLauncher.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "IWeaponSystem.h"
#include "MonoFwd.h"

namespace game
{
    class RocketLauncher : public IWeaponSystem
    {
    public:
        RocketLauncher(mono::EventHandler& handler);

        virtual void Fire(const math::Vector2f& position, float direction);
        virtual const char* SpriteFile() const;

    private:

        mono::EventHandler& mEventHandler;
    };
}
