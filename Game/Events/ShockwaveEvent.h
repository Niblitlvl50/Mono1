//
//  ShockwaveEvent.h
//  MonoOSX
//
//  Created by Niklas Damberg on 16/11/14.
//
//

#pragma once

#include "MonoFwd.h"
#include "Vector2f.h"

namespace game
{
    struct ShockwaveEvent
    {
        ShockwaveEvent(const math::Vector2f& position, float magnitude)
            : mPosition(position),
              mMagnitude(magnitude)
        { }

        const math::Vector2f mPosition;
        const float mMagnitude;
    };

    typedef std::function<bool (const game::ShockwaveEvent&)> ShockwaveEventFunc;
}
