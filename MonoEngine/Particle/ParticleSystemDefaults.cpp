
#include "ParticleSystemDefaults.h"
#include "ParticlePool.h"
#include "Random.h"

void mono::DefaultGenerator(const math::Vector& position, ParticlePool& pool, size_t index, const void* context)
{
    const float x = mono::Random(-80.0f, 80.0f);
    const float y = mono::Random(-80.0f, 80.0f);
    const float life = mono::Random(0.0f, 500.0f);

    pool.m_position[index] = position;
    pool.m_velocity[index] = math::Vector(x, y);
    //pool.m_color[index];
    pool.m_start_color[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
    pool.m_end_color[index] = mono::Color::RGBA(0.0f, 1.0f, 0.0f, 0.1f);
    pool.m_start_size[index] = 10.0f;
    pool.m_end_size[index] = 5.0f;
    pool.m_start_life[index] = 1000 + life;
    pool.m_life[index] = 1000 + life;
}

void mono::DefaultUpdater(ParticlePool& pool, size_t count, unsigned int delta, const void* context)
{
    const float float_delta = float(delta) / 1000.0f;

    for(size_t index = 0; index < count; ++index)
    {
        const float t = 1.0f - float(pool.m_life[index]) / float(pool.m_start_life[index]);

        pool.m_position[index] += pool.m_velocity[index] * float_delta;
        pool.m_color[index] = mono::Color::Lerp(pool.m_start_color[index], pool.m_end_color[index], t);
        pool.m_size[index] = (1.0f - t) * pool.m_start_size[index] + t * pool.m_end_size[index];
    }
}
