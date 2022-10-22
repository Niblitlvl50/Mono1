
#include "Random.h"
#include <random>

using namespace mono;

RandomGenerator::RandomGenerator(int seed)
    : m_random_engine(seed)
{ }

float RandomGenerator::GetFloat(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(m_random_engine);
}

int RandomGenerator::GetInteger(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_random_engine);
}

namespace
{
    constexpr int seed = 666;
    static RandomGenerator g_random_generator(seed);
}

float mono::Random(float min, float max)
{
    return g_random_generator.GetFloat(min, max);
}

int mono::RandomInt(int min, int max)
{
    return g_random_generator.GetInteger(min, max);
}
