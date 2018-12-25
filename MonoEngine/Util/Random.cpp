
#include "Random.h"
#include <random>

namespace
{
    constexpr int seed = 666;
    static std::default_random_engine engine(seed);
}

float mono::Random(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(engine);
}

int mono::RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(engine);
}
