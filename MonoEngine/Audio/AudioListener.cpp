//
//  AudioListener.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 14/06/16.
//
//

#include "AudioListener.h"
#include "Math/Vector.h"
#include "OpenAL/al.h"

void mono::ListenerPosition(const math::Vector& position)
{
    alListener3f(AL_POSITION, position.x, position.y, 0.0f);
}

void mono::ListenerGain(float gain)
{
    alListenerf(AL_GAIN, gain);
}
