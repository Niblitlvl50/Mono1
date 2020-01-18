
#pragma once

namespace mono
{
    void InitializeAudio();
    void ShutdownAudio();

    // Set the listener position
    void ListenerPosition(float x, float y);

    // Set the listener volume (gain)
    void ListenerGain(float gain);
}
