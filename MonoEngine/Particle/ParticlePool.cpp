
#include "ParticlePool.h"

using namespace mono;

ParticlePool::ParticlePool(int pool_size, ParticleUpdater update_function)
    : m_poolSize(pool_size),
      m_update_function(update_function)
{
    m_position.resize(pool_size);
    m_velocity.resize(pool_size);
    m_color.resize(pool_size);
    m_startColor.resize(pool_size);
    m_endColor.resize(pool_size);
    m_startLife.resize(pool_size);
    m_life.resize(pool_size);
}

void ParticlePool::Kill(size_t index)
{
    if(m_countAlive > 0)
    {
        --m_countAlive;
        Swap(index, m_countAlive);
    }
}

void ParticlePool::Wake(size_t index)
{
    if(m_countAlive < m_poolSize)
    {
        Swap(index, m_countAlive);
        ++m_countAlive;
    }
}

void ParticlePool::Swap(size_t first, size_t second)
{
    std::swap(m_position[first], m_position[second]);
    std::swap(m_velocity[first], m_velocity[second]);
    std::swap(m_color[first], m_color[second]);
    std::swap(m_startColor[first], m_startColor[second]);
    std::swap(m_endColor[first], m_endColor[second]);
    std::swap(m_startLife[first], m_startLife[second]);
    std::swap(m_life[first], m_life[second]);
}

void ParticlePool::doUpdate(unsigned int delta)
{
    m_update_function(*this, m_countAlive, delta);

    for(size_t index = 0; index < m_countAlive; ++index)
    {
        int& life = m_life[index];
        life -= delta;
        if(life <= 0)
            Kill(index);
    }
}
