
#include "ParticleSystemDefaults.h"
#include "ParticlePool.h"
#include "Utils.h"

void mono::DefaultGenerator(const math::Vector& position, ParticlePool& pool, size_t index)
{
    const float x = mono::Random(-80.0f, 80.0f);
    const float y = mono::Random(-80.0f, 80.0f);
    const float life = mono::Random(0.0f, 500.0f);

    pool.m_position[index] = position;
    pool.m_velocity[index] = math::Vector(x, y);
    //pool.m_color[index];
    pool.m_startColor[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
    pool.m_endColor[index] = mono::Color::RGBA(0.0f, 1.0f, 0.0f, 0.1f);
    pool.m_startLife[index] = 1000 + life;
    pool.m_life[index] = 1000 + life;
}

void mono::DefaultUpdater(ParticlePool& pool, size_t count, unsigned int delta)
{
    const float float_delta = float(delta) / 1000.0f;

    for(size_t index = 0; index < count; ++index)
    {
        const float t = 1.0f - float(pool.m_life[index]) / float(pool.m_startLife[index]);

        pool.m_position[index] += pool.m_velocity[index] * float_delta;
        pool.m_color[index] = mono::Color::Lerp(pool.m_startColor[index], pool.m_endColor[index], t);
    }
}
