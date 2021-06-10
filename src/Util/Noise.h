
#pragma once

namespace mono
{
    class Noise
    {
    public:

        static float Perlin(float x, float y);
        static float PerlinFbm(float x, float y);
    };
}
