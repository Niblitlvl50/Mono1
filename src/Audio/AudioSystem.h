
#pragma once

namespace mono
{
    void InitializeAudio();
    void ShutdownAudio();

    void ListenerPosition(float x, float y);
    void ListenerVelocity(float x, float y);
    void ListenerDirection(float direction);

    // Set the listener volume (gain)
    void ListenerGain(float gain);
}
