
#pragma once

#include <random>
#include <cstdint>

namespace mono
{
    class RandomGenerator
    {
    public:
        RandomGenerator(int seed);
        float GetFloat(float min = 0.0f, float max = 1.0f);
        int GetInteger(int min = 0, int max = 100);

    private:
        std::default_random_engine m_random_engine;
    };

    float Random(float min = 0.0f, float max = 1.0f);
    int RandomInt(int min = 0, int max = 100);

    // Specify the percentage for the outcome to be true, 100 means always true, 20 means 20% of the time its true.
    inline bool Chance(int percentage)
    {
        return (RandomInt() < percentage);
    }

    class UniformRandomBitGenerator
    {
    public:

        using result_type = uint32_t;

        UniformRandomBitGenerator(int seed)
            : m_random_generator(seed)
        { }

        static constexpr result_type min()
        {
            return 0;
        }

        static constexpr result_type max()
        {
            return 100;
        }

        result_type operator()()
        {
            return m_random_generator.GetInteger(min(), max());
        }

        RandomGenerator m_random_generator;
    };
}
