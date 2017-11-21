
#pragma once

#include "Particle/ParticleFwd.h"
#include "Math/Vector.h"
#include "Rendering/Color.h"
#include <vector>

namespace mono
{
    class ParticlePool
    {
    public:

        ParticlePool(int pool_size, ParticleUpdater update_function);

        void Kill(size_t index);
        void Wake(size_t index);
        void Swap(size_t first, size_t second);

        void doUpdate(unsigned int delta);

        std::vector<math::Vector> m_position;
        std::vector<math::Vector> m_velocity;
        std::vector<Color::RGBA> m_color;
        std::vector<Color::RGBA> m_startColor;
        std::vector<Color::RGBA> m_endColor;
        std::vector<int> m_startLife;
        std::vector<int> m_life;

        size_t m_poolSize = 0;
        size_t m_countAlive = 0;
        ParticleUpdater m_update_function;
    };
}
