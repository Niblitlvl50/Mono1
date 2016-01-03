//
//  StandardWeapon.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "BaseWeapon.h"
#include "MonoPtrFwd.h"

namespace game
{
    class StandardWeapon : public BaseWeapon
    {
    public:

        StandardWeapon(mono::EventHandler& eventHandler);

    private:
        
        virtual void DoFire(const math::Vector2f& position, float direction) const;
        virtual int RoundsPerSecond() const;

        mono::ISoundPtr m_sound;
    };
}
