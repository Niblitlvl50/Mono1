
#include "AudioSystem.h"
#include "System/System.h"

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
