
#pragma once

namespace mono
{
    //! Generates a random float between min and max
    float Random(float min = 0.0f, float max = 1.0f);

    int RandomInt(int min = 0, int max = 100);

    // Specify the percentage for the outcome to be true, 100 means always true, 20 means 20% of the time its true.
    inline bool Chance(int percentage)
    {
        return (RandomInt() < percentage);
    }
}
