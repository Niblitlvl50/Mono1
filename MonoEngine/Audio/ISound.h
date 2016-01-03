//
//  ISound.h
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#pragma once

#include <memory>

namespace mono
{
    class ISound
    {
    public:

        virtual ~ISound()
        { }

        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
    };

    typedef std::shared_ptr<ISound> ISoundPtr;
}