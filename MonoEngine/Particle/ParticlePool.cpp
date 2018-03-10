
#include "ParticlePool.h"

using namespace mono;

ParticlePool::ParticlePool(int pool_size, ParticleUpdater update_function, const void* update_context)
    : m_pool_size(pool_size)
    , m_update_function(update_function)
    , m_update_context(update_context)
{
    m_position.resize(pool_size);
    m_velocity.resize(pool_size);
    m_color.resize(pool_size);
    m_start_color.resize(pool_size);
    m_end_color.resize(pool_size);
    m_size.resize(pool_size);
    m_start_size.resize(pool_size);
    m_end_size.resize(pool_size);
    m_start_life.resize(pool_size);
    m_life.resize(pool_size);
}

void ParticlePool::Kill(size_t index)
{
    if(m_count_alive > 0)
    {
        --m_count_alive;
        Swap(index, m_count_alive);
    }
}

void ParticlePool::Wake(size_t index)
{
    if(m_count_alive < m_pool_size)
    {
        Swap(index, m_count_alive);
        ++m_count_alive;
    }
}

void ParticlePool::Swap(size_t first, size_t second)
{
    std::swap(m_position[first], m_position[second]);
    std::swap(m_velocity[first], m_velocity[second]);
    std::swap(m_color[first], m_color[second]);
    std::swap(m_start_color[first], m_start_color[second]);
    std::swap(m_end_color[first], m_end_color[second]);
    std::swap(m_start_life[first], m_start_life[second]);
    std::swap(m_life[first], m_life[second]);
}

void ParticlePool::doUpdate(unsigned int delta)
{
    m_update_function(*this, m_count_alive, delta, m_update_context);

    for(size_t index = 0; index < m_count_alive; ++index)
    {
        int& life = m_life[index];
        life -= delta;
        if(life <= 0)
            Kill(index);
    }
}
