
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
        virtual bool IsPlaying() const = 0;
        virtual float SampleLength() const = 0;

        virtual void Pitch(float pitch) = 0;
        virtual void Gain(float gain) = 0;

        virtual void Position(float x, float y) = 0;
    };
}
