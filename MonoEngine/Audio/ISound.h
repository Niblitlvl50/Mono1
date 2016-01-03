//
//  ISound.h
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#pragma once

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
}