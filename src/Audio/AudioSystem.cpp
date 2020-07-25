
#include "AudioSystem.h"
#include "System/System.h"

#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include "open_al.h"

#include <stdexcept>
#include <cstdio>

namespace
{
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
}

void mono::InitializeAudio()
{
    // Pass in nullptr to get the default device
    device = alcOpenDevice(nullptr);
    if(!device)
        throw std::runtime_error("Unable to open OpenAL device");

    context = alcCreateContext(device, nullptr);
    if(!context)
        throw std::runtime_error("Unable to create OpenAL context");

    alcMakeContextCurrent(context);

    const ALCchar* defaultDevice = alcGetString(device, ALC_DEFAULT_DEVICE_SPECIFIER);
    const ALchar* vendor = alGetString(AL_VENDOR);
    const ALchar* version = alGetString(AL_VERSION);
    const ALchar* renderer = alGetString(AL_RENDERER);

    System::Log("OpenAL\n"
                "\tvendor: %s\n"
                "\tdevice: %s\n"
                "\trenderer: %s\n"
                "\tversion: %s\n", vendor, defaultDevice, renderer, version);
}

void mono::ShutdownAudio()
{
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void mono::ListenerPosition(float x, float y)
{
    alListener3f(AL_POSITION, x, y, 0.0f);
}

void mono::ListenerVelocity(float x, float y)
{
    alListener3f(AL_VELOCITY, x, y, 0.0f);
    const ALenum error_enum = alGetError();
    if(error_enum != AL_NO_ERROR)
    {
        System::Log("OPEN AL ERROR: %d\n", error_enum);
    }
}

void mono::ListenerDirection(float direction)
{
    const math::Vector at_vector = math::VectorFromAngle(direction);

    const float values[] = {
        at_vector.x, at_vector.y, 0.0f,  // At vector
        0.0f, 0.0f, 1.0f    // Up vector
    };

    alListenerfv(AL_ORIENTATION, values);
}

void mono::ListenerGain(float gain)
{
    alListenerf(AL_GAIN, gain);
}
