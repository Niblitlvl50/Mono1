
#include "AudioListener.h"
#include "OpenAL/al.h"

void mono::ListenerPosition(float x, float y)
{
    alListener3f(AL_POSITION, x, y, 0.0f);
}

void mono::ListenerGain(float gain)
{
    alListenerf(AL_GAIN, gain);
}
