//
//  SysSound.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#include "SysSound.h"
#include "OpenAL/alc.h"

#include <stdexcept>
#include <cstdio>

namespace
{
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
}

void Sound::Init()
{
    // Pass in nullptr to get the default device
    device = alcOpenDevice(nullptr);
    if(!device)
        throw std::runtime_error("Unable to open OpenAL device");

    context = alcCreateContext(device, nullptr);
    if(!context)
        throw std::runtime_error("Unable to create OpenAL context");

    alcMakeContextCurrent(context);

    const ALCchar* bytes = alcGetString(device, ALC_DEFAULT_DEVICE_SPECIFIER);

    ALCint major;
    ALCint minor;
    alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &major);
    alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &minor);

    std::printf("OpenAL device: %s, version: %i.%i\n", bytes, major, minor);
}

void Sound::Exit()
{
    alcDestroyContext(context);
    alcCloseDevice(device);
}

