
#include "Noise.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"

using namespace mono;

float Noise::Perlin(float x, float y)
{
    return stb_perlin_noise3(x, y, 1.0f, 0, 0, 0);
}

float Noise::PerlinFbm(float x, float y)
{
    constexpr float lacunarity = 6.0f;
    constexpr float gain = 0.5f;
    constexpr int octaves = 6;
    return stb_perlin_fbm_noise3(x, y, 1.0f, lacunarity, gain, octaves);
}
