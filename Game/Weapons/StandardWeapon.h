//
//  StandardWeapon.h
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
    class StandardWeapon : public IWeaponSystem
    {
    public:

        StandardWeapon(mono::EventHandler& eventHandler);

        virtual void Fire(const math::Vector2f& position, float direction);
        virtual int RoundsPerSecond() const;
        virtual const char* SpriteFile() const;

    private:

        mono::EventHandler& mEventHandler;
    };
}
